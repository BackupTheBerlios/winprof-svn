#pragma once

#include "stdafx.h"
#include <hash_map>
#include <list>
#include <dbghelp.h>
#include "..\CallMon\CallMon.h"

class CSymbolManager
{
	friend BOOL CALLBACK SymbolsCallback(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext);
public:
	void SetProcess(HANDLE hProcess, const std::list<CALL_INFO>& call_info);
	void Flush();
	CSymbolManager();
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
	void EnumSymbols(const std::list<CALL_INFO>& call_info);
	
	typedef stdext::hash_map<DWORD, CString> symbol_map_t;
	symbol_map_t symbols;
};
