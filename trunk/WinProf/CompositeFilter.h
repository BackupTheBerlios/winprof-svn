#pragma once

#include "Filter.h"

class CCompositeFilter : public CFilter
{
public:
	CCompositeFilter(CString ex, CString nm, const CFilter* v1, const CFilter* v2, logical_oper op)
		: CFilter(nm)
		, f1(v1)
		, f2(v2)
		, oper(op)
		, expr(ex)
	{ASSERT((v2!=NULL && oper != LogicalOper::NOT) || (v2==NULL && oper == LogicalOper::NOT));}

	// delete all intermediate filters, which don't appear in the hash
	virtual ~CCompositeFilter(void) 
	{
		ASSERT(f1 != NULL);
		if (f1->GetExpr() == "") delete f1;
		if (f2 != NULL) {
			if (f2->GetExpr() == "") delete f2;
		}
	}

	virtual bool IsDependantOn(CString n) const
		{return DoesContain(n);}

	virtual bool IsAtom(void) const
		{return false;}

	virtual CString GetExpr(void) const
		{return expr;}
	virtual SetExpr(CString e) 
		{expr = e;}

	// make use of a filter
	virtual bool Satisfies(const INVOC_INFO& iv) const
		{return oper(f1, f2, iv);}

#ifdef _DEBUG_FILTER
	CString GetOperSymbol(void) const {
		if(oper == LogicalOper::NOT) {
			return "~";
		} else if (oper == LogicalOper::AND) {
			return "&";
		} else if (oper == LogicalOper::OR) {
			return "|";
		} else {
			ASSERT(false);
		}
		return CString();
	}
	virtual CString BuildExpression(void) const
	{
		CString result;
		ASSERT(f1 != NULL);
		if (f2 != NULL) {
			result.Format("( %s ) %s ( %s )", f1->BuildExpression(), GetOperSymbol(), f2->BuildExpression());
		} else {
			result.Format("( %s %s )", GetOperSymbol(), f1->BuildExpression());
		}
		return result;
	}
#endif

public:
	virtual bool DoesContain(CString n) const
	{
		bool dep = f1->DoesContain(n);
		if (f2 != NULL) dep = (dep || f2->DoesContain(n));
		return dep;
	}

private:
	const CFilter* f1;
	const CFilter* f2;
	logical_oper oper;
	CString expr;
};