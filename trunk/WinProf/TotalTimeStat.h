#pragma once

#include "WinProfStatistics.h"
#include "FunctionTreeView.h"

class CTotalTimeStat : public CWinProfStatistics 
{
public:
	CTotalTimeStat(void) {}
	virtual ~CTotalTimeStat(void) {}

	virtual CString GetString(const INVOC_INFO& call) const
		{return CFunctionTreeView::dword64tostr(GetStatValue(call));}

	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const 
		{return CWinProfStatistics::StatCompare<DWORD64>(GetStatValue(c1), GetStatValue(c2));}

	virtual CString GetStatName(void) const 
		{return "TotalTime";}

	virtual stats_type GetStatID(void) const
		{return TOTAL_TIME;}

private:
	typedef stdext::hash_map<DWORD/*address*/, DWORD64/*time*/> total_time_map_t;
	typedef total_time_map_t::const_iterator iter_t;
	//static total_time_map_t total_time_map;

protected:
	DWORD64 GetStatValue(const INVOC_INFO& call) const
	{
		func2vect_t::const_iterator vect_it = func2vect.find(call.address);
		if(vect_it == func2vect.end()) return -1;
		
		const calls_vector_t* call_vect = vect_it->second;
		ASSERT(call_vect != NULL);
		DWORD64 total_time = 0;
		for(int i = 0; i < (int)call_vect->size(); i++)
			total_time += (*call_vect)[i]->runtime;

		//TRACE("inside total run time %s \n", CFunctionTreeView::dword64tostr(total_time));

		return total_time;
	}
}; // class CTotalTimeStat