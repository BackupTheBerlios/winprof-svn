#pragma once

#include "WinProfStatistics.h"
#include "FunctionTreeView.h"
#include "WinProfDoc.h"
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

class CTotalTimeStat : public CFunctionStat<DWORD64, CStatHelperSum>
{
public:
	CTotalTimeStat(void) : CFunctionStat<DWORD64, CStatHelperSum>(CStatHelperSum()) {}

	virtual CString GetString(const INVOC_INFO& call) const
		{return DWORD64ToStr(GetStatValue(call)*1000/CWinProfDoc::m_Frequency);}

	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const 
		{return CWinProfStatistics::StatCompare<DWORD64>(GetStatValue(c1), GetStatValue(c2));}

	virtual CString GetStatName(void) const 
		{return "Total Time (ms)";}

	virtual stats_type GetStatID(void) const
		{return TOTAL_TIME;}
};
