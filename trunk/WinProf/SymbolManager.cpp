#include "StdAfx.h"
#include ".\symbolmanager.h"
#include <dbghelp.h>
#include <psapi.h>
#include <set>

using namespace std;
using namespace stdext;

void SYMBOL_INFORMATION::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << name << module;
	}
	else
	{
		ar >> name >> module;
	}
}

CSymbolManager::CSymbolManager(HANDLE hProcess_)
	: hProcess(hProcess_), hModules(NULL), szModPath(NULL), nModCount(0)
{
	SetProcess(hProcess_);
}

void CSymbolManager::SetProcess(HANDLE hProcess)
{
	if (hProcess == NULL) return;
	this->hProcess = hProcess;
	SymSetOptions(SYMOPT_UNDNAME | SYMOPT_LOAD_LINES | SYMOPT_DEFERRED_LOADS);
	EnumModules();
	CString path = GetModulesPaths();
	char *p = path.GetBuffer();
	SymInitialize(hProcess, p, TRUE);
	path.ReleaseBuffer();
	EnumSymbols();
}

void CSymbolManager::Flush()
{
	delete [] hModules;
	delete [] szModPath;
	hModules = NULL;
	szModPath = NULL;
	if (hProcess)
	{
		hProcess = NULL;
		SymCleanup(hProcess);
	}
}

CSymbolManager::~CSymbolManager(void)
{
	Flush();
}

void CSymbolManager::EnumModules()
{
	if (hProcess == NULL) return;
	HMODULE hmod;
	DWORD cbNeeded;
	if (!EnumProcessModules(hProcess, &hmod, sizeof(hmod), &cbNeeded))
	{
		MessageBox(NULL, _T("Can't enumerate process modules"), NULL, MB_ICONERROR);
		return;
	}
	nModCount = cbNeeded / sizeof(HMODULE);
	hModules = new HMODULE [nModCount];
	szModPath = new CString [nModCount];
	if (!EnumProcessModules(hProcess, hModules, cbNeeded, &cbNeeded))
	{
		MessageBox(NULL, _T("Can't enumerate process modules"), NULL, MB_ICONERROR);
		return;
	}
}

CString CSymbolManager::GetModulesPaths()
{
	if (hProcess == NULL) return CString();
	CString path;
	set<CString> paths;
	for (int i = 0; i < nModCount; i++)
	{
		char szModName[MAX_PATH];
		if (GetModuleFileNameEx(hProcess, hModules[i], szModName, sizeof(szModName)))
		{
			szModPath[i] = szModName;
			char *pLastSlash = strrchr(szModName, '\\');
			if (pLastSlash)
				*pLastSlash = '\0';
			paths.insert(CString(szModName).MakeLower());
		}
	}
	for (set<CString>::const_iterator iter = paths.begin(); iter != paths.end(); ++iter)
		(path += ";" ) += *iter;
	return path.Mid(1);
}

CString CSymbolManager::GetSymName(DWORD dwAddress)
{
	symbol_map_t::const_iterator iter = symbols.find(dwAddress);
	if (iter == symbols.end()) return CString();
	return iter->second.name;
}

BOOL CALLBACK SymbolsCallback(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext)
{
	pair<void *, int> *p = reinterpret_cast<pair<void *, int> *>(UserContext);
	CSymbolManager *symman = reinterpret_cast<CSymbolManager *>(p->first);
	SYMBOL_INFORMATION syminfo;
	syminfo.name = pSymInfo->Name;
	syminfo.module = p->second;
	symman->symbols[(DWORD)pSymInfo->Address] = syminfo;
	return TRUE;
}

void CSymbolManager::EnumSymbols()
{
	if (hProcess == NULL) return;
	for (int i=0; i<nModCount; i++)
	{
		pair<void *, int> p(this, i);
		SymEnumSymbols(hProcess, (ULONG64)hModules[i], NULL, SymbolsCallback, &p);
	}
}

void CSymbolManager::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << nModCount;
		for (int i=0; i<nModCount; i++)
			ar << szModPath[i];
		ar << (unsigned int)symbols.size();
		for (symbol_map_t::iterator iter = symbols.begin(); iter != symbols.end(); ++iter)
		{
			ar << iter->first;
			iter->second.Serialize(ar);
		}
	}
	else
	{
		ar >> nModCount;
		if (szModPath) delete [] szModPath;
		szModPath = new CString [nModCount];
		for (int i=0; i<nModCount; i++)
			ar >> szModPath[i];
		unsigned int size;
		ar >> size;
		while (size--)
		{
			DWORD address;
			SYMBOL_INFORMATION si;
			ar >> address;
			si.Serialize(ar);
			symbols[address] = si;
		}
	}
}
