#ifndef AVG_TIME_STAT
#define AVG_TIME_STAT

#include "TotalTimeStat.h"
#include "CountCallsStat.h"

namespace WIN_PROF_STATISTICS {

class AvgTimeStat : public WinProfStatistics<DWORD64> {
public:
	AvgTimeStat(const TotalTimeStat& ref_total_time_, const CountCallsStat& ref_count_calls_) :
	  ref_total_time(ref_total_time_), ref_count_calls(ref_count_calls_) {}
	virtual ~AvgTimeStat(void) {}

	virtual DWORD64 GetStatValue(const RUN_INFO& ri) {
		DWORD64 calls = (DWORD64)ref_count_calls.GetStatValue();
		if (calls == 0) return 0;
		return ref_total_time.GetStatValue()/calls;
	}
	virtual BOOL UpdateWith(const RUN_INFO& ri) {
		return true;
	}
	virtual comp_func GetCompareFunc(void) {
		return StatCompare<DWORD64>;
	}

private:
	TotalTimeStat& ref_total_time;
	CountCallsStat& ref_count_calls;
}; // class AvgTimeStat 

}; // namespace

#endif // COUNT_CALLS_STAT
