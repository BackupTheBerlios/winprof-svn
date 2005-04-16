#pragma once

#include "TotalTimeStat.h"
#include "CountCallsStat.h"
#include <math.h>

class CAvgTimeStat : public CTotalTimeStat, public CCountCallsStat
{
public:
	CAvgTimeStat(void) {}

	virtual CString GetString(const INVOC_INFO& call) const 
		{return DWORD64ToStr(GetStatValue(call)*1000/CWinProfDoc::m_Frequency);}

	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const
		{return CWinProfStatistics::StatCompare<DWORD64>(GetStatValue(c1), GetStatValue(c2));}

	virtual CString GetStatName(void) const 
		{return "Avg Run Time (ms)";}

	virtual void ClearCache(void) const
		{CTotalTimeStat::ClearCache(); CCountCallsStat::ClearCache();}

protected:
	DWORD64 GetStatValue(const INVOC_INFO& call) const
	{
		DWORD64 calls = CCountCallsStat::GetStatValue(call);
		if (calls == 0) return 0;
		return CTotalTimeStat::GetStatValue(call)/calls;
	}
}; // class CAvgTimeStat 

class CStdDevTimeStat : public CAvgTimeStat, public CTotalSquaredTimeStat
{
public:
	CStdDevTimeStat(void) {}

	virtual CString GetString(const INVOC_INFO& call) const 
		{return DWORD64ToStr(GetStatValue(call));}

	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const
		{return CWinProfStatistics::StatCompare<DWORD64>(GetStatValue(c1), GetStatValue(c2));}

	virtual CString GetStatName(void) const 
		{return "Std. Dev. of Run Time (ms)";}

	virtual void ClearCache(void) const
		{CAvgTimeStat::ClearCache(); CCountCallsStat::ClearCache();}

protected:
	DWORD64 GetStatValue(const INVOC_INFO& call) const
	{
		int calls = CCountCallsStat::GetStatValue(call);
		if (calls == 0) return 0;
		DWORD64 avg = CAvgTimeStat::GetStatValue(call);
		double squared = CTotalSquaredTimeStat::GetStatValue(call);
		double variance = squared/calls - avg*avg;
		return (DWORD64)(sqrt(variance) / CWinProfDoc::m_Frequency * 1000);
	}
}; // class CStdDevTimeStat 
