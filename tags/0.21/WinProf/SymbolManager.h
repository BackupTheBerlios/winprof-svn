#pragma once

#include "stdafx.h"
#include <hash_map>
#include <dbghelp.h>

struct SYMBOL_INFORMATION
{
	CString name;
	HMODULE module;
};

class CSymbolManager
{
	friend BOOL CALLBACK SymbolsCallback(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext);
public:
	CSymbolManager(HANDLE hProcess=0);
	void SetProcess(HANDLE hProcess);
	~CSymbolManager(void);
	CString GetSymName(DWORD dwAddress);
private:
	HANDLE hProcess;
	HMODULE *hModules;
	int nModCount;
	CString GetModulesPaths();
	void EnumModules();
	void EnumSymbols();
	
	typedef stdext::hash_map<DWORD, SYMBOL_INFORMATION> symbol_map_t;
	symbol_map_t symbols;
};
