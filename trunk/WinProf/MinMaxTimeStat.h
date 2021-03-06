#pragma once

#include "WinProfStatistics.h"

class CMinTimeStat : public CWinProfStatistics
{
public:
	virtual CString ToString(stat_val_t val) const
		{return DWORD64ToStr((DWORD64)Round((double)val.dw64_val*1000/CWinProfDoc::m_Frequency));}
	virtual CString GetStatCaption(void) const 
		{return "Min Run Time (ms)";}
	virtual string GetStatName() const
		{return "MinTimeStat";}
	virtual bool IsPerFunction(void) const
		{return true;}
	virtual bool IsCacheable() const 
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
	virtual stat_val_t CalculateStatVal(const calls_vector_t& v, const INVOC_INFO& call) const
	{
		stat_val_t val;
		val.dw64_val = 0;
		if (v.empty()) return val;
		DWORD64 min = (DWORD64)-1;
		for (calls_vector_t::const_iterator i = v.begin(); i != v.end(); ++i)
			if (min > i->runtime) min = i->runtime;
		val.dw64_val = min;
		return val;
	}
	virtual stat_val_t CalculateStatVal(const filtered_list_t& list) const
	{
		stat_val_t val;
		val.dw64_val = 0;
		if (list.empty()) return val;
		DWORD64 min = (DWORD64)-1;
		for (filtered_list_t::const_iterator i = list.begin(); i != list.end(); ++i)
		{
			DWORD64 runtime = GetFuncCallStat(*(*i)).runtime;
			if (min > runtime) min = runtime;
		}
		val.dw64_val = min;
		return val;
	}
};

class CMaxTimeStat : public CWinProfStatistics
{
public:
	virtual CString ToString(stat_val_t val) const
		{return DWORD64ToStr((DWORD64)Round((double)val.dw64_val*1000/CWinProfDoc::m_Frequency));}
	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const
		{return CWinProfStatistics::StatCompare<DWORD64>(GetStatValue(c1).dw64_val, GetStatValue(c2).dw64_val);}
	virtual CString GetStatCaption(void) const 
		{return "Max Run Time (ms)";}
	virtual string GetStatName() const
		{return "MaxTimeStat";}
	virtual bool IsPerFunction(void) const
		{return true;}
	virtual bool IsCacheable() const 
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
	virtual stat_val_t CalculateStatVal(const calls_vector_t& v, const INVOC_INFO& call) const
	{
		stat_val_t val;
		val.dw64_val = 0;
		if (v.empty()) return val;
		DWORD64 max = 0;
		for (calls_vector_t::const_iterator i = v.begin(); i != v.end(); ++i)
			if (max < i->runtime) max = i->runtime;
		val.dw64_val = max;
		return val;
	}
	virtual stat_val_t CalculateStatVal(const filtered_list_t& list) const
	{
		stat_val_t val;
		val.dw64_val = 0;
		if (list.empty()) return val;
		DWORD64 max = 0;
		for (filtered_list_t::const_iterator i = list.begin(); i != list.end(); ++i)
		{
			DWORD64 runtime = GetFuncCallStat(*(*i)).runtime;
			if (max < runtime) max = runtime;
		}
		val.dw64_val = max;
		return val;
	}
};
