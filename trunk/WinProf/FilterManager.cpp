#include "stdafx.h"
#include "FilterManager.h"
#include "AtomFilter.h"
#include "MainFrm.h"

filname2fil_t CFilterManager::filname2fil;

void CFilterManager::Destroy(void)
{
	filname2fil_t::iterator it = filname2fil.begin();
	for(; it != filname2fil.end(); ++it) {
CFilter* filt = it->second;
CString str;
str.Format("%lu filter named %s to be deleted, Destroy(void)\n", (void*)filt, filt->GetName());
OutputDebugString(str);
		delete it->second;
	}
	filname2fil.clear();
}

bool CFilterManager::Destroy(CString name)
{
	if (! CanDestroy(name)) return false;
	filname2fil_t::iterator it = filname2fil.find((const char*)name);
	if(it != filname2fil.end()) {
CFilter* filt = it->second;
CString str;
str.Format("%lu filter named %s to be deleted, Destroy(name)\n", (void*)filt, filt->GetName());
OutputDebugString(str);
		delete it->second;
		filname2fil.erase(it);
	}
	return true;
}

bool CFilterManager::CanDestroy(CString name)
{
	filname2fil_t::iterator it = filname2fil.begin();
	for(; it != filname2fil.end(); ++it) {
		if (it->first != (const char*)name && it->first[0] != '$' && it->second->IsDependantOn(name))
			return false;
	}
	return true;
}

void CFilterManager::GetFilterNames(vector<CString>& containter) 
{
	ASSERT(containter.empty() == true);
	filname2fil_t::iterator it = filname2fil.begin();
	for(; it != filname2fil.end(); ++it) {
		if(it->first[0] != '$')
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
	if (!ValidForStat(name, IsValidName)) return false;
	filname2fil_t::const_iterator it = filname2fil.find((const char*)name);
	if (it != filname2fil.end()) return false;

	vector<CString> infix, postfix;
	BuildInfix(expr, infix);
	BuildPostfix(infix, postfix); infix.clear();
	CFilter* fil = CreateFilterByPostfix(name, postfix);

	if (fil == NULL) return false;
	CString msg;
	msg.Format("update expr with %s for %s\n", expr, fil->GetName());
	OutputDebugString(msg);
	fil->SetExpr(expr); 

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
CString str;
str.Format("%lu filter named %s to be deleted, EditCompFilter\n", (void*)filt, filt->GetName());
OutputDebugString(str);
		delete filt; // the new filter is added
		return true;
	} else {
		filname2fil[(const char*)filt->GetName()] = filt; // insert it back
		return false;
	}
}

bool CFilterManager::AddFilter(CString name, bool this_f, DWORD fn, int st, stat_val_t bnd, cmp_oper op)
{
	if (!ValidForStat(name, IsValidName)) return false;
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
CString str;
str.Format("%lu filter named %s to be deleted, EditAtomFilter\n", (void*)filt, filt->GetName());
OutputDebugString(str);
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

void CFilterManager::FailedToCreate(vector<CString>& stack)
{
	// with the new implementation we have already placed them in the stack
/*	for (int i = 0; i < (int)stack.size(); ++i) {
		delete stack[i];
	} */ 
}

CFilter* CFilterManager::CreateFilterByPostfix(CString new_filter, const vector<CString>& postfix)
{
	vector<CString> stack;
	CString token, fil, f1, f2;
	CFilter* fil_ptr;
	bool flag = false;

	for(int i = 0; i < (int)postfix.size(); i++) {
		token = postfix[i];
		if(IsOper(token[0])) {
			if ((int)stack.size() < 1) {
				FailedToCreate(stack); 
				return NULL;
			} else {
				f1 = stack.back(); stack.pop_back();
				f2 = "";
				if (token[0] == '|' || token[0] == '&') {
					f2 = stack.back(); stack.pop_back();
					// since all operators are symmetric, we could do it an easier way
					fil_ptr = GetFilter(f2)->CreateNew("", CFilter::GetTmpName(), f1, GetLogOpID(token[0]));
				} else {
					fil_ptr = GetFilter(f1)->CreateNew("", CFilter::GetTmpName(), "", GetLogOpID(token[0]));
				}
				flag = true;
				ASSERT(GetFilter((const char*)fil_ptr->GetName()) == NULL);
				filname2fil[(const char*)fil_ptr->GetName()] = fil_ptr;
				stack.push_back(fil_ptr->GetName()); // the temporary name given
			}
		} else {
			fil_ptr = GetFilter(token);
			if (fil_ptr == NULL || fil_ptr->IsDependantOn(new_filter)) {
				FailedToCreate(stack);
				return NULL;
			}
			else {
				stack.push_back(token);
			}
		}
	}
	if ((int)stack.size() != 1) {
		FailedToCreate(stack);
		return NULL;
	}
	fil = stack.back();
	fil_ptr = GetFilter(fil);
	if (!flag) {
		// create the filter
		fil_ptr = fil_ptr->CreateNew("", CFilter::GetTmpName(), fil, LogicalOper::AND);
	} else {
		// erase the entry with tmp_name(the last one)
		filname2fil_t::iterator it = filname2fil.find((const char*)fil_ptr->GetName());
		ASSERT(it != filname2fil.end());
		filname2fil.erase(it);
	}

	filname2fil[(const char*)new_filter] = fil_ptr;
	fil_ptr->SetName(new_filter);

	return fil_ptr; // the name and the expression to be updated later
}

bool CFilterManager::IndependantOn(const vector<CString>& stack, CFilter* filt)
{
	vector<CString>::const_iterator it = stack.begin();
	for(; it != stack.end(); ++it) 
		if(*it != filt->GetName() && filt->IsDependantOn(*it)) return false;
	return true;
}

void CFilterManager::Linearize(vector<CString>& stack)
{
	ASSERT(stack.empty() == true);

	vector<CString> stack1, stack2;
	GetFilterNames(stack1);

	vector<CString>* current = &stack1; // contains all names remained to insert to stack
	vector<CString>* remained = &stack2; // contains reminders after each loop

	bool changed = false; // if when we detect that current is not changed after a pass - then we cannot linearize
	
	while(! current->empty()) {
		changed = false;
		vector<CString>::iterator it = current->begin();
		for(; it != current->end(); ++it) {
			if(IndependantOn(*current, GetFilter(*it)) && IndependantOn(*remained, GetFilter(*it))) {
				stack.push_back(*it);
				changed = true;
			} else {
				remained->push_back(*it);
			}
		}
		ASSERT(changed);
		// swap remaned and current
		current->clear();
		vector<CString>* tmp = current;
		current = remained;
		remained = tmp;
	}
}

void CFilterManager::Serialize(CArchive& ar)
{
	vector<CString> stack;
	if (ar.IsStoring())
	{
		Linearize(stack);
		ar << (unsigned int)stack.size();
		CFilter* filt;

		vector<CString>::const_iterator it = stack.begin();
		for(; it != stack.end(); ++it) {
			filt = GetFilter(*it);
			ASSERT(filt != NULL);
			if (filt->IsAtom()) {
				DWORD fn;
				bool this_func;
				int st;
				stat_val_t bnd;
				cmp_oper op; 
				static_cast<CAtomFilter*>(filt)->GetAttrs(fn, this_func,st, bnd, op);
				ar << char('a') << filt->GetName() << fn << this_func << st;
				ar.Write(&bnd, sizeof(stat_val_t));
				ar << (int)op;
			} else {
				ar << char('c') << filt->GetName() << filt->GetExpr();
			}
		}
	}
	else
	{
		Destroy(); // clears the map
		unsigned int size;
		char atom;
		ar >> size;
		while (size--)
		{
			ar >> atom;
			if (atom == 'a') {
				CString name;
				DWORD fn;
				bool this_func;
				int st;
				stat_val_t bnd;
				int op; 
				ar >> name >> fn >> this_func >> st;
				ar.Read(&bnd, sizeof(stat_val_t));
				ar >> op;
				VERIFY(AddFilter(name, this_func, fn, st, bnd, (cmp_oper)op));
			} else { // atom == 'c'
				CString name, expr;
				ar >> name >> expr;
				VERIFY(AddFilter(name, expr));
			}
		}
	}
}
