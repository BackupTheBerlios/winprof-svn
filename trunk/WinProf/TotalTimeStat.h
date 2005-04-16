#pragma once

#include "FunctionStat.h"

class CStatHelperSum
{
public:
	DWORD64 operator() (const calls_vector_t& v, const INVOC_INFO& call)
	{
		DWORD64 sum = 0;
		for (calls_vector_t::const_iterator i = v.begin(); i != v.end(); ++i)
			sum += i->runtime;
		return sum;
	}
};

class CStatHelperSumSquared
{
public:
	double operator() (const calls_vector_t& v, const INVOC_INFO& call)
	{
		double sum = 0;
		for (calls_vector_t::const_iterator i = v.begin(); i != v.end(); ++i)
			sum += i->runtime * i->runtime;
		return sum;
	}
};

class CTotalTimeStat : public CFunctionStat<DWORD64, CStatHelperSum>
{
public:
	CTotalTimeStat(void) : CFunctionStat<DWORD64, CStatHelperSum>(CStatHelperSum()) {}
};

class CTotalSquaredTimeStat : public CFunctionStat<double, CStatHelperSumSquared>
{
public:
	CTotalSquaredTimeStat(void) : CFunctionStat<double, CStatHelperSumSquared>(CStatHelperSumSquared()) {}
};
