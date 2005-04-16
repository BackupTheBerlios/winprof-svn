#pragma once

#include "WinProfStatistics.h"
#include "FunctionStat.h"

class CStatHelperMin
{
public:
	DWORD64 operator() (const calls_vector_t& v, const INVOC_INFO& call)
	{
		if (v.empty()) return 0;
		DWORD64 min = (DWORD64)-1;
		for (calls_vector_t::const_iterator i = v.begin(); i != v.end(); ++i)
			if (min > i->runtime) min = i->runtime;
		return min;
	}
};

class CStatHelperMax
{
public:
	DWORD64 operator() (const calls_vector_t& v, const INVOC_INFO& call)
	{
		if (v.empty()) return 0;
		DWORD64 max = 0;
		for (calls_vector_t::const_iterator i = v.begin(); i != v.end(); ++i)
			if (max < i->runtime) max = i->runtime;
		return max;
	}
};

class CMinTimeStat : public CFunctionStat<DWORD64, CStatHelperMin>
{
public:
	CMinTimeStat(void) : CFunctionStat<DWORD64, CStatHelperMin>(CStatHelperMin()) {}

	virtual CString GetString(const INVOC_INFO& call) const
		{return DWORD64ToStr(GetStatValue(call)*1000/CWinProfDoc::m_Frequency);}

	virtual CString GetStatName(void) const 
		{return "Min Run Time (ms)";}
};

class CMaxTimeStat : public CFunctionStat<DWORD64, CStatHelperMax>
{
public:
	CMaxTimeStat(void) : CFunctionStat<DWORD64, CStatHelperMax>(CStatHelperMax()) {}

	virtual CString GetString(const INVOC_INFO& call) const
		{return DWORD64ToStr(GetStatValue(call)*1000/CWinProfDoc::m_Frequency);}

	virtual CString GetStatName(void) const 
		{return "Max Run Time (ms)";}
};
