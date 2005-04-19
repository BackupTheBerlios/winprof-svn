#pragma once

#include "Filter.h"
#include "CmpOper.h"
#include "StatManager.h"
#include "WinProfStatistics.h"

class CAtomFilter : public CFilter
{
public:
	CAtomFilter(DWORD fn, int st, double bnd, cmp_oper op)
		: func(fn)
		, stat(st)
		, bound(bnd)
		, oper(op)
	{}
	virtual ~CAtomFilter(void) {}

	// make use of a filter
	virtual bool FilterOut(const INVOC_INFO& iv) const
	{
		if(func != 0 && func != iv.address) 
			return true;
		if(stat != -1 && !stats[stat]->Satisfies(iv, bound, oper))
			return true;
		return false; // is not filtered OUT
	}

private:
	DWORD func; // hold by address; 0 means no function
	int stat; // hold by statistics number; -1 means no stat
	double bound;
	cmp_oper oper;

public:
	static const statistics_t& stats;
};

const statistics_t& CAtomFilter::stats = CStatManager::GetStats();