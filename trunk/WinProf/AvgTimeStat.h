#pragma once

#include "MainFrm.h"
#include "TotalTimeStat.h"
#include "CountCallsStat.h"

class CAvgTimeStat : public CTotalTimeStat, public CCountCallsStat
{
public:
	CAvgTimeStat(void) {}
	virtual ~CAvgTimeStat(void) {}

	virtual CString GetString(const INVOC_INFO& call) const 
		{return CFunctionTreeView::dword64tostr(GetStatValue(call));}

	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const
		{return CWinProfStatistics::StatCompare<DWORD64>(GetStatValue(c1), GetStatValue(c2));}

	virtual CString GetStatName(void) const 
		{return "AvgTime";}

	virtual stats_type GetStatID(void) const 
		{return AVG_TIME;}

protected:
	DWORD64 GetStatValue(const INVOC_INFO& call) const {
		DWORD64 calls = (DWORD64)CCountCallsStat::GetStatValue(call);
		//TRACE("inside avg run time %d \n", (int)calls);
		if (calls == 0) return DWORD64(0);
		return CTotalTimeStat::GetStatValue(call)/calls;
	}
}; // class CAvgTimeStat 