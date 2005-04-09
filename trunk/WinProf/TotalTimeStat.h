#ifndef TOTAL_TIME_STAT
#define TOTAL_TIME_STAT

#include "WinProfStatistics.h"
#include <hash_map>

namespace WIN_PROF_STATISTICS {

class TotalTimeStat : public WinProfStatistics<DWORD64> {
public:
	TotalTimeStat(void) {}
	virtual ~TotalTimeStat(void) {}

	virtual T GetStatValue(const RUN_INFO& ri) 
	{
		iter_t iter = total_time_map.find(ri.address);
		if (iter == total_time_map.end()) {
			return 0;
		}
		return total_time_map[ri.address];
	}

	virtual BOOL UpdateWith(const RUN_INFO& ri)
	{
		iter_t iter = total_time_map.find(ri.address);
		if (iter == total_time_map.end()) {
			total_time_map[ri.address] = (DWORD64)0;
		}
		total_time_map[ri.address] += ri.diff;
		return true;
	}
	virtual comp_func GetCompareFunc(void)
	{
		return StatCompare<DWORD64>;
	}

private:
	typedef stdext::hash_map<DWORD/*address*/, DWORD64/*time*/> total_time_map_t;
	typedef total_time_map_t::const_iterator iter_t;
	total_time_map_t total_time_map;
}; // class TotalTimeStat

}; // namespace

#endif // TOTAL_TIME_STAT