#pragma once

#include "WinProfStatistics.h"
#include <hash_map>

template <typename T, typename Function>
class CFunctionStat : public CWinProfStatistics
{
public:
	CFunctionStat(Function& func, bool cache=true)
		: function(func), cacheable(cache) {}
	virtual ~CFunctionStat(void) {}

	virtual void ClearCache(void) const
		{cache.clear();}

private:
	typedef stdext::hash_map<INVOC_INFO, T> cache_map_t;
	static cache_map_t cache;
	Function& function;
	bool cacheable, cacheable_per_invoc;

protected:
	T GetStatValue(const INVOC_INFO& call) const
	{
		if (cacheable)
		{
			cache_map_t::const_iterator preserved = cache.find(call);
//			cache_map_t::const_iterator preserved = cacheable_per_invoc?cache.find(call):cache.find(INVOC_INFO(call.address));
			if (preserved != cache.end()) return preserved->second;
		}

		func2vect_t::const_iterator it = func2vect.find(call.address);
		ASSERT(it != func2vect.end());
		if(it == func2vect.end()) return T();
		T value = function(it->second, call);
		if (cacheable) cache[call] = value;
//		if (cacheable) cache[cacheable_per_invoc?call:INVOC_INFO(call.address)] = value;
		return value;
	}
}; // class CFunctionStat