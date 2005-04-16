#pragma once

#include "FunctionStat.h"

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
		{return Format("%d", GetStatValue(call));}

	virtual CString GetStatName(void) const
		{return "Call Count";}
};
