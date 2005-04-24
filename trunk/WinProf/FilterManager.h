#pragma once

#include "Filter.h"
#include "FunctionTreeView.h"
#include "CommonDefs.h"

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
	CFilter* GetFilter(CString name);
	bool AddFilter(CString name, CString expr);
	bool AddFilter(CString name, bool this_f, DWORD fn, int st, stat_val_t bnd, cmp_oper op);
	void Filter(CString name, const CTreeCtrl& ctrl, filtered_list_t& filtered_list);
	bool EditFilter(CString nm, CString new_nm, CString expr);
	bool EditFilter(CString nm, CString new_nm, bool this_f, DWORD fn, int st, stat_val_t bnd, cmp_oper op);

//private: // functions
	bool CanDestroy(CString name);
	void UpdateAllDependant(CFilter* oldf, CFilter* newf);
	CFilter* TakeFilterAside(CString nm);

	CString RemoveBlanksAtEnds(CString str);
	bool IsOper(char c);
	bool IsDelim(char c);
	int FindFirstDelimFrom(CString expr, int from);
	int Prio(char c);
	void BuildInfix(CString expr, vector<CString>& stack);
	void BuildPostfix(vector<CString>& infix, vector<CString>& postfix);

	logical_oper GetLogOpID(char op);
	void FailedToCreate(vector<CFilter*>& stack);
	CFilter* CreateFilterByPostfix(CString new_filter, const vector<CString>& postfix);
#ifdef _DEBUG_FILTER
	void CheckConsistency(CFilter* fil) const;	
#endif

private: // data
	static filname2fil_t filname2fil;
	static const char* delims;
};

