#include "stdafx.h"
#include "FilterManager.h"
#include "AtomFilter.h"
#include "MainFrm.h"

filname2fil_t CFilterManager::filname2fil;

void CFilterManager::Destroy(void)
{
	filname2fil_t::iterator it = filname2fil.begin();
	for(; it != filname2fil.end(); ++it) {
		delete it->second;
		filname2fil.erase(it);
	}
}

void CFilterManager::Destroy(CString name)
{
	filname2fil_t::iterator it = filname2fil.find((const char*)name);
	if(it != filname2fil.end()) {
		delete it->second;
		filname2fil.erase(it);
	}
}

bool CFilterManager::AddFilter(CString name, CString expr)
{
	vector<CString> infix, postfix;
	BuildInfixFormStack(expr, infix);
	BuildPostfix(infix, postfix);
	infix.clear();
	//AddFilterByPostfix(name, postfix);
	return false;
}

bool CFilterManager::AddFilter(CString name, DWORD fn, int st, stat_val_t bnd, cmp_oper op)
{
	filname2fil_t::const_iterator it = filname2fil.find((const char*)name);
	if (it != filname2fil.end()) return false;
	filname2fil[(const char*)name] = new CAtomFilter(fn, st, bnd, op);
	return true;
}

void CFilterManager::Filter(CString name, const CTreeCtrl& ctrl, filtered_list_t& filtered_list)
{
	filname2fil_t::const_iterator it = filname2fil.find((const char*)name);
	if (it == filname2fil.end()) return;
	CFilter* fil = it->second;
	ASSERT(fil != NULL);

	HTREEITEM root = ctrl.GetRootItem(), current = root;
	do {
		const INVOC_INFO* iv = (INVOC_INFO*)(ctrl.GetItemData(current));
		ASSERT(iv != NULL);
		if (fil->Satisfies(*iv)) filtered_list.push_back(iv);

		if (ctrl.ItemHasChildren(current)) {
			current = ctrl.GetNextItem(current, TVGN_CHILD);
		} else {
			while (ctrl.GetNextSiblingItem(current) == NULL && current != root) {
				current = ctrl.GetNextItem(current, TVGN_PARENT);
			}
			if(current != root) {
				current = ctrl.GetNextSiblingItem(current);
			}
		}
	} while(current != root);
}

//typedef enum{LEQ=0, LES, GEQ, GRT, EQV, NEQ, CMP_OPER_COUNT} cmp_oper;
namespace CmpOper 
{
	const char* caption[CMP_OPER_COUNT] = {"<=", "<", ">=", ">", "==", "!="};
	cmp_oper_func_double cmp_oper_vect_double[CMP_OPER_COUNT] = {Leq<double>, Les<double>, Geq<double>, Grt<double>, Eqv<double>, Neq<double>};
	cmp_oper_func_int cmp_oper_vect_int[CMP_OPER_COUNT] = {Leq<int>, Les<int>, Geq<int>, Grt<int>, Eqv<int>, Neq<int>};
	cmp_oper_func_dw64 cmp_oper_vect_dw64[CMP_OPER_COUNT] = {Leq<DWORD64>, Les<DWORD64>, Geq<DWORD64>, Grt<DWORD64>, Eqv<DWORD64>, Neq<DWORD64>};
}; // namespace CmpOper 

/**********************************************
* parsing support
**********************************************/

const char* CFilterManager::delims = "|&~()";

CString CFilterManager::RemoveBlanksAtEnds(CString str) 
{
	int i = 0, len = str.GetLength();
	while(i < len && str[i] == ' ') i++;
	if (i==len) return CString();
	int j = len - 1;
	while(str[j] == ' ') j--;
	return str.Mid(i, j-i+1);
}


bool CFilterManager::IsDelim(char c) 
{
	for(int i = 0; i < (int)strlen(delims); i++)
		if (c == delims[i]) return true;
	return false;
}

bool CFilterManager::IsOper(char c) 
{
	return (IsDelim(c) && c != '(' && c != ')');
}

int CFilterManager::FindFirstDelimFrom(CString expr, int from) 
{
	int len = expr.GetLength();
	while(from < len && !IsDelim(expr[from])) {from++;}
	if (from >= len) return -1;
	return from;
}


void CFilterManager::BuildInfixFormStack(CString expr, vector<CString>& stack) 
{
	int cur = 0, pos;
	CString tmp;
	while((pos = FindFirstDelimFrom(expr, cur)) != -1) 
	{
		if (pos != cur) { // means that op immediately found
			tmp = RemoveBlanksAtEnds(expr.Mid(cur, pos-cur));
			if (tmp != "") stack.push_back(tmp);
		}
		stack.push_back(CString(expr[pos]));
		cur = pos+1;
	}
	int len = expr.GetLength();
	if (cur < len) {
		tmp = RemoveBlanksAtEnds(expr.Mid(cur, len-cur));
		if (tmp != "") stack.push_back(tmp);
	}
}

int CFilterManager::Prio(char c)
{
	switch (c) {
		case '~': return 3;
		case '&': return 2;
		case '|': return 1;
		default: return 0;
	}
}

void CFilterManager::BuildPostfix(vector<CString>& infix, vector<CString>& postfix) 
{
	vector<CString> stack;

	postfix.clear();
	stack.push_back("(");
	infix.push_back(")");

	int index = 0;
	while(! stack.empty()) 
	{
		CString cur = infix[index];
		char first = cur[0];
	
		if(!IsDelim(first)) {
			postfix.push_back(cur);
		} else if (first == '(') {
			stack.push_back(cur);
		} else if (IsOper(first)) {
			CString delim = stack.back();
			while (IsOper(delim[0]) && Prio(delim[0]) >= Prio(first)) {
				postfix.push_back(delim);
				stack.pop_back();
				delim = stack.back();
			}
			stack.push_back(cur);
		} else if (first == ')') {
			CString delim = stack.back();
			while (delim[0] != '(') {
				postfix.push_back(delim);
				stack.pop_back();
				delim = stack.back();
			}
			stack.pop_back();
		}
		index++;
	}
}

//void CFilterManager::AddFilterByPostfix(CString name, vector<CString> postfix);
