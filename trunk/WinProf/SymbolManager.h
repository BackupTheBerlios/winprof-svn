#pragma once

#include "stdafx.h"
#include <hash_map>
#include <dbghelp.h>

struct SYMBOL_INFORMATION
{
	CString name;
	int module;
	void Serialize(CArchive& ar);
};

class CSymbolManager
{
	friend BOOL CALLBACK SymbolsCallback(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext);
public:
	CSymbolManager(HANDLE hProcess=0);
	void SetProcess(HANDLE hProcess);
	void Flush();
	~CSymbolManager(void);
	CString GetSymName(DWORD dwAddress);
	void Serialize(CArchive& ar);
private:
	HANDLE hProcess;
	HMODULE *hModules;
	CString *szModPath;
	int nModCount;
	CString GetModulesPaths();
	void EnumModules();
	void EnumSymbols();
	
	typedef stdext::hash_map<DWORD, SYMBOL_INFORMATION> symbol_map_t;
	symbol_map_t symbols;
};
