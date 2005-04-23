#pragma once

#include "stdafx.h"
#include "Filter.h"
#include "StatManager.h"
#include "WinProfStatistics.h"

class CAtomFilter : public CFilter
{
public:
	CAtomFilter(CString nm, DWORD fn, bool this_func, int st, stat_val_t bnd, cmp_oper op)
		: CFilter(nm)
		, func(fn)
		, only_this_func(this_func)
		, stat(st)
		, bound(bnd)
		, oper(op)
	{}
	virtual ~CAtomFilter(void) {}

	void GetAttrs(DWORD& fn, bool& this_func, int& st, stat_val_t& bnd, cmp_oper& op) 
		{fn = func; this_func = only_this_func; st = stat; bnd = bound; op = oper;}

	virtual bool IsDependantOn(CString n) const
		{return false;}

	virtual bool IsAtom(void) const
		{return true;}

	virtual CString GetExpr(void) const
		{return CString("@");} // this is not an expression, but can serve as a marker


	// make use of a filter
	virtual bool Satisfies(const INVOC_INFO& iv) const
	{
		if(func != 0) {
			if(only_this_func == true && func != iv.address) 
				return false;
			if(only_this_func == false && func == iv.address) 
				return false;
		}
		if(stat != -1 && !stats[stat]->Satisfies(iv, bound, oper))
			return false;
		return true; // the filter didn't kill the invocation
	}

#ifdef _DEBUG_FILTER
	virtual CString BuildExpression(void) const
		{return name;}
#endif

public:
	virtual bool DoesContain(CString n) const
		{return (name == n);}

private:
	DWORD func; // hold by address; 0 means no function
	bool only_this_func; // if false then all functions 'func' are filtered out
	int stat; // hold by statistics number; -1 means no stat
	stat_val_t bound;
	cmp_oper oper; // enum, defined in Filter.cpp

public:
	static const statistics_t& stats;
};
