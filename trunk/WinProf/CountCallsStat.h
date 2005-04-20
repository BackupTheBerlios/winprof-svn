#pragma once

#include "WinProfStatistics.h"

class CCountCallsStat : public CWinProfStatistics
{
	virtual CString GetString(const INVOC_INFO& call) const
		{return Format("%d", GetStatValue(call).int_val);}
	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const
		{return CWinProfStatistics::StatCompare<int>(GetStatValue(c1).int_val, GetStatValue(c2).int_val);}
	virtual CString GetStatName(void) const
		{return "Call Count";}
	virtual bool IsPerFunciton(void) const 
		{return true;}
	
protected:
	// calculate stat value
	virtual stat_val_t CalculateStatVal(const calls_vector_t& v, const INVOC_INFO& call) const
	{
		stat_val_t val;
		val.int_val = (int)v.size();
		return val;
	}
};
