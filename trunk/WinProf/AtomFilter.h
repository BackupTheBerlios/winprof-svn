#pragma once

#include "Filter.h"
#include "StatManager.h"
#include "WinProfStatistics.h"

class CAtomFilter : public CFilter
{
public:
	CAtomFilter(DWORD fn, int st, stat_val_t bnd, cmp_oper op)
		: func(fn)
		, stat(st)
		, bound(bnd)
		, oper(op)
	{}
	virtual ~CAtomFilter(void) {}

	// make use of a filter
	virtual bool Satisfies(const INVOC_INFO& iv) const
	{
		if(func != 0 && func != iv.address) 
			return false;
		if(stat != -1 && !stats[stat]->Satisfies(iv, bound, oper))
			return false;
		return true; // the filter didn't kill the invocation
	}

private:
	DWORD func; // hold by address; 0 means no function
	int stat; // hold by statistics number; -1 means no stat
	stat_val_t bound;
	cmp_oper oper; // enum, defined in CommonDefs.h

public:
	static const statistics_t& stats;
};
