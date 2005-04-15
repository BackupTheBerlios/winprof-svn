#pragma once

#include "WinProfStatistics.h"
#include "FunctionTreeView.h"
#include "WinProfDoc.h"
#include "FunctionStat.h"

class CStatHelperGet
{
public:
	DWORD64 operator() (const calls_vector_t& v, const INVOC_INFO& call)
		{return v[call.invocation-1].runtime;}
};

class CRunTimeStat : public CFunctionStat<DWORD64, CStatHelperGet>
{
public:
	CRunTimeStat(void) : CFunctionStat<DWORD64, CStatHelperGet>(CStatHelperGet(), false) {}

	virtual CString GetString(const INVOC_INFO& call) const
		{return DWORD64ToStr(GetStatValue(call)*1000/CWinProfDoc::m_Frequency);}

	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const 
		{return CWinProfStatistics::StatCompare<DWORD64>(GetStatValue(c1), GetStatValue(c2));}

	virtual CString GetStatName(void) const 
		{return "Run Time (ms)";}

	virtual stats_type GetStatID(void) const
		{return RUN_TIME;}

	virtual bool IsVisible(void) const
		{return true;}
};
