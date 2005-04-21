#pragma once

#include "WinProfDoc.h"
#include "WinProfStatistics.h"

class CRunTimeStat : public CWinProfStatistics 
{
public:
	CRunTimeStat(void) {}

	virtual CString GetString(const INVOC_INFO& call) const
		{return DWORD64ToStr(GetStatValue(call).dw64_val*1000/CWinProfDoc::m_Frequency);}
	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const
		{return CWinProfStatistics::StatCompare<DWORD64>(GetStatValue(c1).dw64_val, GetStatValue(c2).dw64_val);}
	virtual CString GetStatCaption(void) const 
		{return "Run Time (ms)";}
	virtual string GetStatName() const
		{return "RunTimeStat";}
	virtual bool IsPerInvocation(void) const
		{return true;}

protected:
	virtual stat_val_t CalculateStatVal(const calls_vector_t& v, const INVOC_INFO& call) const
	{	
		stat_val_t val;
		val.dw64_val = v[call.invocation-1].runtime;
		return val;
	}
}; 