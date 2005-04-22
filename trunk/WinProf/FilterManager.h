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
	void Destroy(CString name);
	bool AddFilter(CString name, CString expr);
	bool AddFilter(CString name, DWORD fn, int st, stat_val_t bnd, cmp_oper op);
	void Filter(CString name, const CTreeCtrl& ctrl, filtered_list_t& filtered_list);

//private: // functions
	CString RemoveBlanksAtEnds(CString str);
	bool IsOper(char c);
	bool IsDelim(char c);
	int FindFirstDelimFrom(CString expr, int from);
	int Prio(char c);
	void BuildInfixFormStack(CString expr, vector<CString>& stack);
	void BuildPostfix(vector<CString>& infix, vector<CString>& postfix);


	bool NOT(CString n1, CString n2);
	bool AND(CString n1, CString n2);
	bool OR(CString n1, CString n2);

private: // data
	static filname2fil_t filname2fil;
	static const char* delims;
};

