#include "stdafx.h"
#include "FilterManager.h"
#include "AtomFilter.h"
#include "MainFrm.h"

filname2fil_t CFilterManager::filname2fil;

void CFilterManager::Destroy(void)
{
	filname2fil_t::iterator it = filname2fil.begin();
	for(; it != filname2fil.end(); ++it) {
		if (! it->second->IsAtom()) {
			delete it->second;
			it->second = NULL;
		}
	}
	it = filname2fil.begin();
	for(; it != filname2fil.end(); ++it) {
		if (it->second != NULL)
			delete it->second;
	}
	filname2fil.clear();
}

bool CFilterManager::Destroy(CString name)
{
	if (! CanDestroy(name)) return false;
	filname2fil_t::iterator it = filname2fil.find((const char*)name);
	if(it != filname2fil.end()) {
		delete it->second;
		filname2fil.erase(it);
	}
	return true;
}

bool CFilterManager::CanDestroy(CString name)
{
	filname2fil_t::iterator it = filname2fil.begin();
	for(; it != filname2fil.end(); ++it) {
		if (it->first != (const char*)name && it->second->IsDependantOn(name))
			return false;
	}
	return true;
}

void CFilterManager::UpdateAllDependant(CFilter* oldf, CFilter* newf)
{
	filname2fil_t::iterator it = filname2fil.begin();
	for(; it != filname2fil.end(); ++it) {
		OutputDebugString("from inside UpdateAllDependant\n");
		CString msg;
		msg.Format("filter: %s from UpdateAllDependant\n", it->second->GetName());
		OutputDebugString(msg);
		it->second->UpdateTree(oldf, newf);
	}
}

void CFilterManager::GetFilterNames(vector<CString>& containter) 
{
	ASSERT(containter.empty() == true);
	filname2fil_t::iterator it = filname2fil.begin();
	for(; it != filname2fil.end(); ++it) {
		containter.push_back((it->first).c_str());
	}
}

CFilter* CFilterManager::GetFilter(CString name)
{
	filname2fil_t::iterator it = filname2fil.find((const char*)name);
	if (it == filname2fil.end()) return NULL;
	return it->second;
}
#ifdef _DEBUG_FILTER
void CFilterManager::CheckConsistency(CFilter* fil) const
{
	CString built_expr = fil->BuildExpression();
	CString orig_expr = fil->GetExpr();
	CString msg;
	msg.Format("built: %s \n orig: %s \n", built_expr, orig_expr);
	MessageBox(NULL, msg, NULL, MB_OK);
}
#endif

bool CFilterManager::AddFilter(CString name, CString expr)
{
	if (! CFilter::ValidName(name)) return false;
	filname2fil_t::const_iterator it = filname2fil.find((const char*)name);
	if (it != filname2fil.end()) return false;

	vector<CString> infix, postfix;
	BuildInfix(expr, infix);
	BuildPostfix(infix, postfix); infix.clear();
	CFilter* fil = CreateFilterByPostfix(name, postfix);
	if (fil == NULL) return false;
	
	fil->SetExpr(expr); 
	fil->SetName(name);
	filname2fil[(const char*)name] = fil;
#ifdef _DEBUG_FILTER
	CheckConsistency(fil);	
#endif
	return true;
}

CFilter* CFilterManager::TakeFilterAside(CString nm)
{
	filname2fil_t::iterator it = filname2fil.find((const char*)nm);	
	if (it == filname2fil.end()) return NULL; // the filter is not found
	CFilter* filt = it->second;
	filname2fil.erase(it); // the filter is taken aside
	return filt;
}

bool CFilterManager::EditFilter(CString nm, CString new_nm, CString expr)
{
	if (nm != new_nm && !CanDestroy(nm)) return false;
	CFilter* filt = TakeFilterAside(nm);
	if (filt == NULL) return false;
	if (AddFilter(new_nm, expr)) {
		// dependancies are by pointers, so the pointers must be replaced if needed
		// if the name is changed, then no dependancies detected
		// it is important to do it before the deletion ??? not at all (for safety is done so)
		if (nm == new_nm) {
			CString msg;
			msg.Format("editing composite filter %s, update from %lu to %lu\n", nm, filt, GetFilter(new_nm));
			OutputDebugString(msg);
			UpdateAllDependant(filt, GetFilter(new_nm));
		}
		delete filt; // the new filter is added
		return true;
	} else {
		filname2fil[(const char*)filt->GetName()] = filt; // insert it back
		return false;
	}
}

bool CFilterManager::AddFilter(CString name, bool this_f, DWORD fn, int st, stat_val_t bnd, cmp_oper op)
{
	if (! CFilter::ValidName(name)) return false;
	filname2fil_t::const_iterator it = filname2fil.find((const char*)name);
	if (it != filname2fil.end()) return false;

	filname2fil[(const char*)name] = new CAtomFilter(name, fn, this_f, st, bnd, op);
	return true;
}

bool CFilterManager::EditFilter(CString nm, CString new_nm, bool this_f, DWORD fn, int st, stat_val_t bnd, cmp_oper op)
{
	if (nm != new_nm && !CanDestroy(nm)) return false;
	CFilter* filt = TakeFilterAside(nm);
	if (filt == NULL) return false;
	if (AddFilter(new_nm, this_f, fn, st, bnd, op)) {
		// dependancies are by pointers, so the pointers must be replaced if needed
		// if the name is changed, then no dependancies detected
		// it is important to do it before the deletion ??? not at all (for safety is done so)
		if (nm == new_nm) {
			CString msg;
			msg.Format("editing atom filter %s, update from %lu to %lu\n", nm, filt, GetFilter(new_nm));
			OutputDebugString(msg);
			UpdateAllDependant(filt, GetFilter(new_nm));
		}
		delete filt; // the new filter is added
		return true;
	} else {
		filname2fil[(const char*)filt->GetName()] = filt; // insert it back
		return false;
	}
}

void CFilterManager::Filter(CString name, const CTreeCtrl& ctrl, filtered_list_t& filtered_list)
{
	CFilter* fil = NULL;
	if (name != "") {
		filname2fil_t::const_iterator it = filname2fil.find((const char*)name);
		if (it == filname2fil.end()) return;
		fil = it->second;
	}

	HTREEITEM root = ctrl.GetRootItem(), current = root;
	do {
		if (current != root) {
			const INVOC_INFO* iv = (INVOC_INFO*)(ctrl.GetItemData(current));
			ASSERT(iv != NULL);
			if (fil != NULL) {
				if (fil->Satisfies(*iv)) filtered_list.push_back(iv);
			} else {
				filtered_list.push_back(iv);
			}
		}

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
	/*
	int i = 0, len = str.GetLength();
	while(i < len && str[i] == ' ') i++;
	if (i==len) return CString();
	int j = len - 1;
	while(str[j] == ' ') j--;
	return str.Mid(i, j-i+1);
	*/
	return str.Trim();
}


bool CFilterManager::IsDelim(char c) 
{
	/*	
	for(int i = 0; i < (int)strlen(delims); i++)
		if (c == delims[i]) return true;
	return false;
	*/
	return strchr(delims, c) != NULL;
}

bool CFilterManager::IsOper(char c) 
{
	return (IsDelim(c) && c != '(' && c != ')');
}

int CFilterManager::FindFirstDelimFrom(CString expr, int from) 
{
	/*
	int len = expr.GetLength();
	while(from < len && !IsDelim(expr[from])) {from++;}
	if (from >= len) return -1;
	return from;
	*/
	int pos = expr.Mid(from).FindOneOf(delims);
	if (pos == -1) return -1;
	return pos + from;
}


void CFilterManager::BuildInfix(CString expr, vector<CString>& stack) 
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
		default: ASSERT(false); return 0;
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

logical_oper CFilterManager::GetLogOpID(char op) {
	switch (op) {
		case '~': return LogicalOper::NOT;
		case '|': return LogicalOper::OR;
		case '&': return LogicalOper::AND;
		default: ASSERT(false);
	}
	return LogicalOper::NOT;
}

void CFilterManager::FailedToCreate(vector<CFilter*>& stack)
{
	for (int i = 0; i < (int)stack.size(); ++i) {
		delete stack[i];
	}
}

CFilter* CFilterManager::CreateFilterByPostfix(CString new_filter, const vector<CString>& postfix)
{
	vector<CFilter*> stack;
	CString name;
	CFilter *fil, *f1, *f2;
	bool flag = false;

	for(int i = 0; i < (int)postfix.size(); i++) {
		name = postfix[i];
		if(IsOper(name[0])) {
			if ((int)stack.size() < 2) {
				FailedToCreate(stack);
				return NULL;
			} else {
				f1 = stack.back(); stack.pop_back();
				f2 = NULL;
				if (name[0] == '|' || name[0] == '&') {
					f2 = stack.back(); stack.pop_back();
					// since all operators are symmetric, we could do it an easier way
					fil = f2->CreateNew("", "", f1, GetLogOpID(name[0]));
				} else {
					fil = f1->CreateNew("", "", NULL, GetLogOpID(name[0]));
				}
				flag = true;
				stack.push_back(fil);
			}
		} else {
			fil = GetFilter(name);
			if (fil == NULL || fil->IsDependantOn(new_filter)) {
				FailedToCreate(stack);
				return NULL;
			}
			else {
				stack.push_back(fil);
			}
		}
	}
	if ((int)stack.size() != 1) {
		FailedToCreate(stack);
		return NULL;
	}
	fil = stack.back();
	if (!flag) fil = fil->CreateNew("", "", fil, LogicalOper::AND);
	return fil;
}
