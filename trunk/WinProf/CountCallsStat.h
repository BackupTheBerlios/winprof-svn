#pragma once

#include "WinProfStatistics.h"
#include <hash_map>

class CCountCallsStat : public CWinProfStatistics
{
public:
	CCountCallsStat(void) {}
	virtual ~CCountCallsStat(void) {}

	virtual CString GetString(const INVOC_INFO& call) const
		{char buf[100]; return CString(itoa(GetStatValue(call), buf, 10));}

	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const
		{return CWinProfStatistics::StatCompare<INT>(GetStatValue(c1), GetStatValue(c2));}

	virtual CString GetStatName(void) const
		{return "CountCalls";}

	virtual stats_type GetStatID(void) const
		{return COUNT_CALLS;}

private:
	typedef stdext::hash_map<DWORD/*address*/, INT/*calls*/> count_calls_map_t;
	typedef count_calls_map_t::const_iterator iter_t;
	//static count_calls_map_t count_calls_map;

protected:
	INT GetStatValue(const INVOC_INFO& call) const
	{
		func2vect_t::const_iterator it = func2vect.find(call.address);
		if(it == func2vect.end()) return INT(-1);
		return INT(it->second->size()); // number of invocations
	}
}; // class CCountCallsStat