#pragma once

#include "WinProfStatistics.h"

class CTotalTimeStat : public CWinProfStatistics
{
	virtual CString GetString(const INVOC_INFO& call) const 
		{return DWORD64ToStr(GetStatValue(call).dw64_val*1000/CWinProfDoc::m_Frequency);}
	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const
		{return CWinProfStatistics::StatCompare<DWORD64>(GetStatValue(c1).dw64_val, GetStatValue(c2).dw64_val);}
	virtual string GetStatName(void) const 
		{return "TotalTimeStat";}
	virtual bool IsCacheable() const 
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
	virtual stat_val_t CalculateStatVal(const calls_vector_t& v, const INVOC_INFO& call) const
	{
		stat_val_t val;
		val.dw64_val = 0;
		for (calls_vector_t::const_iterator i = v.begin(); i != v.end(); ++i)
			val.dw64_val += i->runtime;
		return val;
	}
};

class CTotalSquaredTimeStat : public CWinProfStatistics
{
	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const
		{return CWinProfStatistics::StatCompare<double>(GetStatValue(c1).double_val, GetStatValue(c2).double_val);}
	virtual string GetStatName(void) const 
		{return "TotalSquaredTimeStat";}
	virtual bool IsCacheable() const 
		{return true;}
	virtual bool Satisfies(const INVOC_INFO& iv, stat_val_t bound, cmp_oper oper) const
		{return CmpOper::cmp_oper_vect_double[oper](GetStatValue(iv).double_val, bound.double_val);}
	virtual stat_val_t GetNumerical(CString str) const
	{
		stat_val_t val;
		val.double_val = atof(str);
		return val;
	}

protected:
	virtual stat_val_t CalculateStatVal(const calls_vector_t& v, const INVOC_INFO& call) const
	{
		stat_val_t val;
		val.double_val = 0;
		for (calls_vector_t::const_iterator i = v.begin(); i != v.end(); ++i)
			val.double_val += (double)i->runtime * i->runtime;
		return val;
	}
};
