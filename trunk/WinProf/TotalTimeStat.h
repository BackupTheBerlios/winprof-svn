#pragma once

#include "WinProfStatistics.h"
//#include <hash_map>

class CTotalTimeStat : public CWinProfStatistics 
{
public:
	CTotalTimeStat(func2vect_t& func2vect_) : func2vect(func2vect_) {}
	virtual ~CTotalTimeStat(void) {}

	virtual CString GetString(const INVOC_INFO& call) const
	{
		return CFunctionTreeView::dword64tostr(GetStatValue(call));
	}

	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const {
		return ::StatCompare(GetStatValue(c1), GetStatValue(c2));
	}

	virtual CString GetStatName(void) const 
	{
		return "TotalTime";
	}

private:
	typedef stdext::hash_map<DWORD/*address*/, DWORD64/*time*/> total_time_map_t;
	typedef total_time_map_t::const_iterator iter_t;
	//static total_time_map_t total_time_map;

	func2vect_t& func2vect;

protected:
	DWORD64 GetStatValue(const INVOC_INFO& call) const
	{
		func2vect_t::const_iterator vect_it = func2vect.find(call.address);
		if(vect_it == func2vect.end()) {
			return -1;
		}
		
		const calls_vector_t* call_vect = func2vect[call.address];
		ASSERT(call_vect != NULL);
		DWORD64 total_time = 0;
		calls_vector_t::const_iterator call_it = call_vect->begin();
		for(;call_it != call_vect->end(); ++call_it) {
			FUNC_CALL_STAT* call_ptr = *call_it;
			total_time += call_ptr->runtime;
		}
		return total_time;
	}
}; // class CTotalTimeStat