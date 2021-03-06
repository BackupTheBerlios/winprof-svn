#pragma once

#include "TotalTimeStat.h"
#include "CountCallsStat.h"
#include <math.h>

class CAvgTimeStat : public CWinProfStatistics
{
public:
	virtual CString ToString(stat_val_t val) const 
		{return DWORD64ToStr(val.dw64_val*1000/CWinProfDoc::m_Frequency);}
	virtual CString GetStatCaption(void) const 
		{return "Avg Run Time (ms)";}
	virtual string GetStatName() const
		{return "AvgTimeStat";}
	virtual bool IsPerFunction(void) const 
		{return true;}
	virtual bool Satisfies(const INVOC_INFO& iv, stat_val_t bound, cmp_oper oper) const
		{return CmpOper::cmp_oper_vect_dw64[oper](GetStatValue(iv).dw64_val, bound.dw64_val);}
	virtual bool GetNumerical(CString str, stat_val_t& val) const
	{
		if (!ValidForStat(str, IsDigit)) return false;
		val.dw64_val = _atoi64(str) * CWinProfDoc::m_Frequency / 1000;
		return true;
	}


protected:
	stat_val_t CalculateStatVal(const calls_vector_t& v, const INVOC_INFO& call) const
	{
		DWORD64 time = stats[CStatManager::GetStName2Index()["TotalTimeStat"]]->GetStatValue(call).dw64_val;
		int calls = stats[CStatManager::GetStName2Index()["CountCallsStat"]]->GetStatValue(call).int_val;
		stat_val_t val;
		if (calls)
			val.dw64_val = time/calls;
		else
			val.dw64_val = 0;
		return val;
	}

	virtual stat_val_t CalculateStatVal(const filtered_list_t& list) const
	{
		DWORD64 time = stats[CStatManager::GetStName2Index()["TotalTimeStat"]]->CalculateStatVal(list).dw64_val;
		int calls = stats[CStatManager::GetStName2Index()["CountCallsStat"]]->CalculateStatVal(list).int_val;
		stat_val_t val;
		if (calls)
			val.dw64_val = time/calls;
		else
			val.dw64_val = 0;
		return val;
	}
}; // class CAvgTimeStat 

class CStdDevTimeStat : public CWinProfStatistics
{
public:
	virtual CString ToString(stat_val_t val) const 
		{return DWORD64ToStr(val.dw64_val);}
	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const
		{return CWinProfStatistics::StatCompare<DWORD64>(GetStatValue(c1).dw64_val, GetStatValue(c2).dw64_val);}
	virtual CString GetStatCaption(void) const 
		{return "Std. Dev. of Run Time (ms)";}
	virtual string GetStatName() const
		{return "StdDevTimeStat";}
	virtual bool IsPerFunction(void) const 
		{return true;}
	virtual bool Satisfies(const INVOC_INFO& iv, stat_val_t bound, cmp_oper oper) const
		{return CmpOper::cmp_oper_vect_dw64[oper](GetStatValue(iv).dw64_val, bound.dw64_val);}
	virtual stat_val_t GetNumerical(CString str) const
	{
		stat_val_t val;
		val.dw64_val = _atoi64(str);
		return val;
	}

protected:
	stat_val_t CalculateStatVal(const calls_vector_t& v, const INVOC_INFO& call) const
	{
		stat_val_t val;
		val.dw64_val = 0;
		int calls = stats[CStatManager::GetStName2Index()["CountCallsStat"]]->GetStatValue(call).int_val;
		if (calls == 0) return val;
		DWORD64 avg = stats[CStatManager::GetStName2Index()["AvgTimeStat"]]->GetStatValue(call).dw64_val;
		double squared = stats[CStatManager::GetStName2Index()["TotalSquaredTimeStat"]]->GetStatValue(call).double_val;
		double variance = squared/calls - (double)avg*avg;
		val.dw64_val = (DWORD64)(sqrt(variance) / CWinProfDoc::m_Frequency * 1000);
		return val;
	}

	virtual stat_val_t CalculateStatVal(const filtered_list_t& list) const
	{
		stat_val_t val;
		val.dw64_val = 0;
		int calls = stats[CStatManager::GetStName2Index()["CountCallsStat"]]->CalculateStatVal(list).int_val;
		if (calls == 0) return val;
		DWORD64 avg = stats[CStatManager::GetStName2Index()["AvgTimeStat"]]->CalculateStatVal(list).dw64_val;
		double squared = stats[CStatManager::GetStName2Index()["TotalSquaredTimeStat"]]->CalculateStatVal(list).double_val;
		double variance = squared/calls - (double)avg*avg;
		val.dw64_val = (DWORD64)(sqrt(variance) / CWinProfDoc::m_Frequency * 1000);
		return val;
	}
}; // class CStdDevTimeStat 
