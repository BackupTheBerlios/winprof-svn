#pragma once

#include "Filter.h"
#include "FunctionTreeView.h"
#include "CommonDefs.h"
#include "stdafx.h"

using namespace CmpOper;
using namespace std;

class CFilterManager
{
public:
	CFilterManager(void) {}
	~CFilterManager(void) {Destroy();}

	void Destroy(void);
	bool Destroy(CString name);
	void GetFilterNames(vector<CString>& containter); 
	bool AddFilter(CString name, CString expr);
	bool AddFilter(CString name, bool this_f, DWORD fn, int st, stat_val_t bnd, cmp_oper op);
	bool EditFilter(CString nm, CString new_nm, CString expr);
	bool EditFilter(CString nm, CString new_nm, bool this_f, DWORD fn, int st, stat_val_t bnd, cmp_oper op);
	void Filter(CString name, const CTreeCtrl& ctrl, filtered_list_t& filtered_list);
	void Serialize(CArchive& ar);
	static CFilter* GetFilter(CString name);

private: // functions
	bool CanDestroy(CString name);
	CFilter* TakeFilterAside(CString nm);

	static CString RemoveBlanksAtEnds(CString str);
	static bool IsOper(char c);
	static bool IsDelim(char c);
	static int FindFirstDelimFrom(CString expr, int from);
	static int Prio(char c);
	static void BuildInfix(CString expr, vector<CString>& stack);
	static void BuildPostfix(vector<CString>& infix, vector<CString>& postfix);
	static logical_oper GetLogOpID(char op);
	static void FailedToCreate(vector<CString>& stack);
	static CFilter* CreateFilterByPostfix(CString new_filter, const vector<CString>& postfix);

	bool IndependantOn(const vector<CString>& stack, CFilter* filt);
	void Linearize(vector<CString>& stack);

private: // data
	static filname2fil_t filname2fil;
	static const char* delims;

public:
#ifdef _DEBUG_FILTER
	void CheckConsistency(CFilter* fil) const;	
#endif
};

