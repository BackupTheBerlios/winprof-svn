#pragma once

#include "FunctionStat.h"
#include "WinProfDoc.h"

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

	virtual CString GetStatName(void) const 
		{return "Run Time (ms)";}

	virtual bool IsPerInvocation(void) const
		{return true;}
};
