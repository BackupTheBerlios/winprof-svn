#include "StdAfx.h"
#include ".\symbolmanager.h"
#include <dbghelp.h>
#include <psapi.h>
#include <set>

using namespace std;
using namespace stdext;

CSymbolManager::CSymbolManager()
	: hProcess(NULL), hModules(NULL), szModPath(NULL), nModCount(0)
{}

void CSymbolManager::SetProcess(HANDLE hProcess, const list<CALL_INFO>& call_info)
{
	if (hProcess == NULL) return;
	this->hProcess = hProcess;
	hModules = NULL;
	szModPath = NULL;
	SymSetOptions(SYMOPT_UNDNAME | SYMOPT_LOAD_LINES | SYMOPT_DEFERRED_LOADS);
	EnumModules();
	CString path = GetModulesPaths();
	char *p = path.GetBuffer();
	SymInitialize(hProcess, p, TRUE);
	path.ReleaseBuffer();
	EnumSymbols(call_info);
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
	return iter->second;
}

void CSymbolManager::EnumSymbols(const list<CALL_INFO>& call_info)
{
	if (hProcess == NULL) return;
	DWORD disp;
	char buf[sizeof(IMAGEHLP_SYMBOL) + MAX_SYM_NAME];
	IMAGEHLP_SYMBOL *sym = reinterpret_cast<IMAGEHLP_SYMBOL*>(buf);
	sym->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
	sym->MaxNameLength = MAX_SYM_NAME;
	for (list<CALL_INFO>::const_iterator iter = call_info.begin(); iter != call_info.end(); ++iter)
		if (symbols.find(iter->address) == symbols.end())
			if (SymGetSymFromAddr(hProcess, iter->address, &disp, sym))
				symbols[iter->address] = sym->Name;
}

void CSymbolManager::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << (unsigned int)symbols.size();
		for (symbol_map_t::iterator iter = symbols.begin(); iter != symbols.end(); ++iter)
			ar << iter->first << iter->second;
	}
	else
	{
		unsigned int size;
		ar >> size;
		while (size--)
		{
			DWORD address;
			CString name;
			ar >> address >> name;
			symbols[address] = name;
		}
	}
}
