#pragma once

#include "MainFrm.h"
#include "TotalTimeStat.h"
#include "CountCallsStat.h"
#include "WinProfDoc.h"

class CAvgTimeStat : public CTotalTimeStat, public CCountCallsStat
{
public:
	CAvgTimeStat(void) {}
	virtual ~CAvgTimeStat(void) {}

	virtual CString GetString(const INVOC_INFO& call) const 
		{return DWORD64ToStr(GetStatValue(call)*1000/CWinProfDoc::m_Frequency);}

	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const
		{return CWinProfStatistics::StatCompare<DWORD64>(GetStatValue(c1), GetStatValue(c2));}

	virtual CString GetStatName(void) const 
		{return "Avg Run Time (ms)";}

	virtual stats_type GetStatID(void) const 
		{return AVG_TIME;}

protected:
	DWORD64 GetStatValue(const INVOC_INFO& call) const
	{
		DWORD64 calls = (DWORD64)CCountCallsStat::GetStatValue(call);
		if (calls == 0) return DWORD64(0);
		return CTotalTimeStat::GetStatValue(call)/calls;
	}
}; // class CAvgTimeStat 