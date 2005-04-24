#pragma once

#include "Filter.h"
#include "FilterManager.h"

class CCompositeFilter : public CFilter
{
public:
	CCompositeFilter(CString ex, CString nm, CString v1, CString v2, logical_oper op)
		: CFilter(nm)
		, n1(v1)
		, n2(v2)
		, oper(op)
		, expr(ex)
	{
		//ASSERT(ValidName(n1));
		//ASSERT((ValidName(n2) && v2!="" && oper != LogicalOper::NOT) || (v2=="" && oper == LogicalOper::NOT));
	}
	virtual ~CCompositeFilter(void) {}

	virtual bool IsDependantOn(CString n) const {return DoesContain(n);}
	virtual bool IsAtom(void) const {return false;}
	virtual CString GetExpr(void) const {return expr;}
	virtual void SetExpr(CString e) {expr = e;}
	virtual bool Satisfies(const INVOC_INFO& iv) const
		{return oper(CFilterManager::GetFilter(n1), CFilterManager::GetFilter(n2), iv);}
	virtual bool DoesContain(CString n) const
	{
		CFilter* f1 = CFilterManager::GetFilter(n1);
		CFilter* f2 = CFilterManager::GetFilter(n2);

		// happens when we edit a filter that depends on itself
		// it is not found in the data base
		if(f1 == NULL) {
			OutputDebugString("f1 == NULL\n");
			return true;
		}
		
		return (name == n || f1->DoesContain(n) || (f2 != NULL && f2->DoesContain(n)));
	}

private:
	CString n1, n2, expr;
	logical_oper oper;

public:
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
};