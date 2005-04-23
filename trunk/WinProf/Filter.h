#pragma once

#include "CommonDefs.h"

class CFilter
{
public:
	CFilter(CString n) : name(n) {}
	virtual ~CFilter(void) {}

	// creation of new object, dynamic allocation used
	CFilter* CreateNew(CString expr, CString n, const CFilter* f, logical_oper oper);
	virtual bool IsDependantOn(CString n) const = 0;
	virtual bool IsAtom(void) const = 0;
	virtual CString GetExpr(void) const = 0;
	CString GetName(void) const {return name;}
	void SetName(CString n) {name = n;}
	virtual SetExpr(CString e) {}
	// make use of a filter
	virtual bool Satisfies(const INVOC_INFO& iv) const = 0;
#ifdef _DEBUG_FILTER
	virtual CString BuildExpression(void) const = 0;
#endif

protected:
	CString name;

public: // methods
	virtual bool DoesContain(CString n) const = 0;

	static bool ValidName(CString name) 
	{
		int len = name.GetLength();
		if (len == 0) return false;
		for(int i = 0; i < len; ++i){
			if (! ValidLetterInName(name[i])) return false;
		}
		return true;
	}

private:
	static bool ValidLetterInName(char c) 
	{
		return (
			('a' <= c && c <= 'z') || 
			('A' <= c && c <= 'Z') || 
			('0' <= c && c <= '9') || 
			(c == '_')
		);
	}
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