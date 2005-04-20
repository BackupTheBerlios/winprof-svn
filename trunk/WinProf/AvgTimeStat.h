#pragma once

#include "TotalTimeStat.h"
#include "CountCallsStat.h"
#include <math.h>

class CAvgTimeStat : public CWinProfStatistics
{
public:
	virtual CString GetString(const INVOC_INFO& call) const 
		{return DWORD64ToStr(GetStatValue(call).dw64_val*1000/CWinProfDoc::m_Frequency);}
	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const
		{return CWinProfStatistics::StatCompare<DWORD64>(GetStatValue(c1).dw64_val, GetStatValue(c2).dw64_val);}
	virtual CString GetStatName(void) const 
		{return "Avg Run Time (ms)";}
	virtual bool IsPerFunciton(void) const 
		{return true;}

protected:
	stat_val_t GetStatValue(const INVOC_INFO& call) const
	{
		DWORD64 time = stats[CStatManager::GetStName2Index()["TotalTime"]]->GetStatValue(call).dw64_val;
		int calls = stats[CStatManager::GetStName2Index()["CountCalls"]]->GetStatValue(call).int_val;
		stat_val_t val;
		val.dw64_val = time/calls;
		return val;
	}
}; // class CAvgTimeStat 

class CStdDevTimeStat : public CAvgTimeStat, public CTotalSquaredTimeStat
{
public:
	virtual CString GetString(const INVOC_INFO& call) const 
		{return DWORD64ToStr(GetStatValue(call).dw64_val*1000/CWinProfDoc::m_Frequency);}
	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const
		{return CWinProfStatistics::StatCompare<DWORD64>(GetStatValue(c1).dw64_val, GetStatValue(c2).dw64_val);}
	virtual CString GetStatName(void) const 
		{return "Std. Dev. of Run Time (ms)";}

protected:
	stat_val_t GetStatValue(const INVOC_INFO& call) const
	{
		stat_val_t val;
		val.dw64_val = 0;
		int calls = stats[CStatManager::GetStName2Index()["CountCalls"]]->GetStatValue(call).int_val;
		if (calls == 0) return val;
		DWORD64 avg = stats[CStatManager::GetStName2Index()["AvgTime"]]->GetStatValue(call).dw64_val;
		double squared = (double)stats[CStatManager::GetStName2Index()["TotalSquaredTime"]]->GetStatValue(call).dw64_val;
		double variance = squared/calls - avg*avg;
		val.dw64_val = (DWORD64)(sqrt(variance) / CWinProfDoc::m_Frequency * 1000);
		return val;
	}
}; // class CStdDevTimeStat 
