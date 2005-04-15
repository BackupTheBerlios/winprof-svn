#pragma once

#include "WinProfStatistics.h"
#include "FunctionStat.h"
#include <hash_map>

class CStatHelperSize
{
public:
	int operator()(const calls_vector_t& v, const INVOC_INFO& call) {return (int)v.size();}
};

class CCountCallsStat : public CFunctionStat<int, CStatHelperSize>
{
public:
	CCountCallsStat(void) : CFunctionStat<int, CStatHelperSize>(CStatHelperSize()) {}

	virtual CString GetString(const INVOC_INFO& call) const
		{char buf[100]; return CString(itoa(GetStatValue(call), buf, 10));}

	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const
		{return CWinProfStatistics::StatCompare<int>(GetStatValue(c1), GetStatValue(c2));}

	virtual stats_type GetStatID(void) const
		{return COUNT_CALLS;}

	virtual CString GetStatName(void) const
		{return "Call Count";}
};
