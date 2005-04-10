#pragma once

#include "TotalTimeStat.h"
#include "CountCallsStat.h"

class CAvgTimeStat : public CTotalTimeStat, public CCountCallsStat
{
public:
	CAvgTimeStat(func2vect_t& func2vect_) 
		: CTotalTimeStat(func2vect_), CCountCallsStat(func2vect_) {}
	virtual ~CAvgTimeStat(void) {}

	virtual CString GetString(const INVOC_INFO& call) const {
		return CFunctionTreeView::dword64tostr(GetStatValue(call));
	}
	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const
	{
		return ::StatCompare(GetStatValue(c1), GetStatValue(c2));
	}
	virtual CString GetStatName(void) const {
		return "AvgTime";
	}
protected:
	DWORD64 GetStatValue(const INVOC_INFO& call) const {
		DWORD64 calls = (DWORD64)CCountCallsStat::GetStatValue(call);
		if (calls == 0) return 0;
		return CTotalTimeStat::GetStatValue(call)/calls;
	}
}; // class CAvgTimeStat 