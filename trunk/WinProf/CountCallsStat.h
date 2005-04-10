#pragma once

#include "WinProfStatistics.h"
#include <hash_map>

class CCountCallsStat : public CWinProfStatistics
{
public:
	CCountCallsStat(func2vect_t& func2vect_) : func2vect(func2vect_) {}
	virtual ~CCountCallsStat(void) {}

	virtual CString GetString(const INVOC_INFO& call) const
	{
		char buf[100];
		return CString(itoa(GetStatValue(call), buf, 10));
	}
	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const
	{
		return ::StatCompare(GetStatValue(c1), GetStatValue(c2));
	}
	virtual CString GetStatName(void) const
	{
		return "CountCalls";
	}

private:
	typedef stdext::hash_map<DWORD/*address*/, INT/*calls*/> count_calls_map_t;
	typedef count_calls_map_t::const_iterator iter_t;
	//static count_calls_map_t count_calls_map;

	func2vect_t& func2vect;
protected:
	INT GetStatValue(const INVOC_INFO& call) const
	{
		func2vect_t::const_iterator it = func2vect.find(call.address);
		if(it == func2vect.end()) {
			return -1;
		}
		return (INT)func2vect[call.address]->size(); // number of invocations
	}

}; // class CCountCallsStat