#include "StdAfx.h"
#include ".\symbolmanager.h"
#include <dbghelp.h>
#include <psapi.h>
#include <set>

using namespace std;
using namespace stdext;

CSymbolManager::CSymbolManager(HANDLE hProcess_)
	: hProcess(hProcess_), hModules(NULL), nModCount(0)
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

CSymbolManager::~CSymbolManager(void)
{
	delete [] hModules;
	SymCleanup(hProcess);
}

void CSymbolManager::EnumModules()
{
	HMODULE hmod;
	DWORD cbNeeded;
	if (!EnumProcessModules(hProcess, &hmod, sizeof(hmod), &cbNeeded))
	{
		MessageBox(NULL, _T("Can't enumerate process modules"), NULL, MB_ICONERROR);
		return;
	}
	nModCount = cbNeeded / sizeof(HMODULE);
	hModules = new HMODULE [nModCount];
	if (!EnumProcessModules(hProcess, hModules, cbNeeded, &cbNeeded))
	{
		MessageBox(NULL, _T("Can't enumerate process modules"), NULL, MB_ICONERROR);
		return;
	}
}

CString CSymbolManager::GetModulesPaths()
{
	CString path;
	set<CString> paths;
	for (int i = 0; i < nModCount; i++)
	{
		char szModName[MAX_PATH];
		if (GetModuleFileNameEx(hProcess, hModules[i], szModName, sizeof(szModName)))
		{
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
	if (iter == symbols.end()) return "";
	return iter->second.name;
}

BOOL CALLBACK SymbolsCallback(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext)
{
	pair<void *, int> *p = reinterpret_cast<pair<void *, int> *>(UserContext);
	CSymbolManager *symman = reinterpret_cast<CSymbolManager *>(p->first);
	SYMBOL_INFORMATION syminfo;
	syminfo.name = pSymInfo->Name;
	syminfo.module = symman->hModules[p->second];
	symman->symbols[(DWORD)pSymInfo->Address] = syminfo;
	return TRUE;
}

void CSymbolManager::EnumSymbols()
{
	for (int i=0; i<nModCount; i++)
	{
		pair<void *, int> p(this, i);
		SymEnumSymbols(hProcess, (ULONG64)hModules[i], NULL, SymbolsCallback, &p);
	}
}
