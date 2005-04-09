#ifndef COUNT_CALLS_STAT
#define COUNT_CALLS_STAT

#include "WinProfStatistics.h"
#include <hash_map>

namespace WIN_PROF_STATISTICS {

class CountCallsStat : public WinProfStatistics<INT> {
public:
	CountCallsStat(void) {}
	virtual ~CountCallsStat(void) {}

	virtual T GetStatValue(const RUN_INFO& ri) 
	{
		iter_t iter = count_calls_map.find(ri.address);
		if (iter == count_calls_map.end()) {
			return 0;
		}
		return count_calls_map[ri.address];
	}

	virtual BOOL UpdateWith(const RUN_INFO& ri)
	{
		iter_t iter = count_calls_map.find(ri.address);
		if (iter == count_calls_map.end()) {
			count_calls_map[ri.address] = 0;
		}
		count_calls_map[ri.address]++; // the reference is incremented
		return true;
	}
	virtual comp_func GetCompareFunc(void)
	{
		return StatCompare<INT>;
	}

private:
	typedef stdext::hash_map<DWORD/*address*/, INT/*calls*/> count_calls_map_t;
	typedef count_calls_map_t::const_iterator iter_t;
	count_calls_map_t count_calls_map;
}; // class CountCallsStat

}; // namespace

#endif // COUNT_CALLS_STAT
