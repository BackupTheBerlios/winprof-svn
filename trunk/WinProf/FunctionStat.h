#pragma once

#include "WinProfStatistics.h"
#include <hash_map>

class CFunctionStat : public CWinProfStatistics
{
public:
	CFunctionStat(Function& func, bool cache=true)
		: function(func), cacheable(cache) {}
	virtual ~CFunctionStat(void) {}

	virtual void ClearCache(void) const
		{cache.clear();}

	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const
		{return CWinProfStatistics::StatCompare<T>(GetStatValue(c1), GetStatValue(c2));}

private:
	typedef stdext::hash_map<INVOC_INFO, T> cache_map_t;
	static cache_map_t cache;
	Function& function;
	bool cacheable;

public:
	T GetStatValue(const INVOC_INFO& call) const
	{
		if (cacheable)
		{
			cache_map_t::const_iterator preserved = cache.find(call);
			if (preserved != cache.end()) return preserved->second;
		}

		func2vect_t::const_iterator it = func2vect.find(call.address);
		ASSERT(it != func2vect.end());
		if (it == func2vect.end()) return T();
		T value = function(it->second, call);
		if (cacheable) cache[call] = value;
		return value;
	}
}; // class CFunctionStat