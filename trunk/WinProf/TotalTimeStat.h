#pragma once

#include "WinProfStatistics.h"

class CTotalTimeStat : public CWinProfStatistics
{
	virtual string GetStatName(void) const 
		{return "TotalTimeStat";}
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
		for (calls_vector_t::const_iterator i = v.begin(); i != v.end(); ++i)
			val.dw64_val += i->runtime;
		return val;
	}
	virtual stat_val_t CalculateStatVal(const filtered_list_t& list) const
	{
		stat_val_t val;
		val.dw64_val = 0;
		for (filtered_list_t::const_iterator i = list.begin(); i != list.end(); ++i)
			val.dw64_val += GetFuncCallStat(*(*i)).runtime;
		return val;
	}
};

class CTotalSquaredTimeStat : public CWinProfStatistics
{
	virtual string GetStatName(void) const 
		{return "TotalSquaredTimeStat";}
	virtual bool IsCacheable() const 
		{return true;}
	virtual bool Satisfies(const INVOC_INFO& iv, stat_val_t bound, cmp_oper oper) const
		{return CmpOper::cmp_oper_vect_double[oper](GetStatValue(iv).double_val, bound.double_val);}

protected:
	virtual stat_val_t CalculateStatVal(const calls_vector_t& v, const INVOC_INFO& call) const
	{
		stat_val_t val;
		val.double_val = 0;
		for (calls_vector_t::const_iterator i = v.begin(); i != v.end(); ++i)
			val.double_val += (double)i->runtime * i->runtime;
		return val;
	}
	virtual stat_val_t CalculateStatVal(const filtered_list_t& list) const
	{
		stat_val_t val;
		val.double_val = 0;
		for (filtered_list_t::const_iterator i = list.begin(); i != list.end(); ++i)
		{
			double d = (double)GetFuncCallStat(*(*i)).runtime;
			val.double_val += d*d;
		}
		return val;
	}
};
