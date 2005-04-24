#pragma once

#include "CommonDefs.h"

class CFilter
{
public:
	CFilter(CString n) : name(n) {}
	virtual ~CFilter(void) {}

	// creation of new object, dynamic allocation used
	CFilter* CreateNew(CString expr, CString n, CString son, logical_oper oper);
	CString GetName(void) const {return name;}
	void SetName(CString n) {name = n;}

	virtual bool IsDependantOn(CString n) const = 0;
	virtual bool IsAtom(void) const = 0;
	virtual bool Satisfies(const INVOC_INFO& iv) const = 0;
	virtual bool DoesContain(CString n) const = 0;
	virtual void SetExpr(CString e) {ASSERT(false);} // should never be called with this implementation
	virtual CString GetExpr(void) const = 0;

	static CString GetTmpName(void) 
	{
		static int counter = 0;
		CString str;
		str.Format("$_%d", counter);
		++counter;
		return str;
	}

protected:
	CString name;

public:
#ifdef _DEBUG_FILTER
	virtual CString BuildExpression(void) const = 0;
#endif
}; // class CFilter

class LogicalOper {
public:

	static bool NOT(const CFilter* f1, const CFilter* f2, const INVOC_INFO& iv)
		{return !f1->Satisfies(iv);}

	static bool AND(const CFilter* f1, const CFilter* f2, const INVOC_INFO& iv)
		{return f1->Satisfies(iv) && f2->Satisfies(iv);}

	static bool OR(const CFilter* f1, const CFilter* f2, const INVOC_INFO& iv)
		{return f1->Satisfies(iv) || f2->Satisfies(iv);}
}; // class LogicalOper