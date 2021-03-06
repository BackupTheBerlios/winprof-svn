#pragma once

#include "StatManager.h"
#include "stdafx.h"
#include <string>

using namespace std;
using namespace CmpOper;

class CWinProfStatistics 
{
public:
	CWinProfStatistics(void) {cache.push_back(stat_cache_t());}
	virtual ~CWinProfStatistics(void) {}

	CString GetString(const INVOC_INFO& call) const {return ToString(GetStatValue(call));} // returns the statistics value as a string
	CString GetString(const filtered_list_t& list) const {return ToString(CalculateStatVal(list));}
	virtual CString ToString(stat_val_t val) const {ASSERT(false); return CString();}
	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const {ASSERT(false); return 0;} // sorting_by support
	virtual CString GetStatCaption(void) const {ASSERT(false); return CString();} // returns the column name, as it appears to user in GUI
	virtual string GetStatName(void) const = 0; // returns stat name to be used by other stats
	virtual int GetWidth(void) const {return 100;} // width of column, as it appears to user in GUI
	virtual int GetColumnPlace(void) const {return LVCFMT_LEFT;} // where in column the text is placed
	virtual bool IsPerInvocation(void) const {return false;} // appears in the left pane?
	virtual bool IsPerFunction(void) const {return false;} // appears in stat_dialog?
	virtual bool Satisfies(const INVOC_INFO& iv, stat_val_t bound, cmp_oper oper) const {ASSERT(false); return false;} // filters support
	virtual bool GetNumerical(CString str, stat_val_t& val) const {ASSERT(false); return false;} // used when user enters a string as the atom filter bound value
	virtual bool IsCacheable() const {return false;} // does the statistic use cache?
	
protected:
	// calculate stat value
	virtual stat_val_t CalculateStatVal(const calls_vector_t& v, const INVOC_INFO& call) const = 0;
public:
	virtual stat_val_t CalculateStatVal(const filtered_list_t& list) const {ASSERT(false); return stat_val_t();}

public:
	stat_val_t GetStatValue(const INVOC_INFO& call) const
	{
		stat_val_t value;
		if (IsCacheable() && SearchCache(call, value)) return value;
		func2vect_t::const_iterator it = func2vect.find(call.address);
		ASSERT(it != func2vect.end());
		value = CalculateStatVal(it->second, call);
		if (IsCacheable()) AddToCache(call, value);
		return value;
	}

	static void ClearCache(void)
		{for (cache_t::iterator i = cache.begin(); i != cache.end(); ++i) i->clear();}
	void SetId(int id) const {this->id = id;}
	// if true is returned, then val is properly updated
	bool SearchCache(const INVOC_INFO& call, stat_val_t& val) const
	{
		stat_cache_t::const_iterator iter = cache[id].find(call);
		if (iter == cache[id].end()) return false;
		val = iter->second;
		return true;
	}

protected:
	// a commonly used compare scenario
	template <typename T> 
	static int StatCompare(const T &t1, const T &t2) {
		if (t1 == t2) return 0;
		return (t1 < t2) ? -1 : 1;
	}

	static const func2vect_t& func2vect;
	static const statistics_t& stats;

	static const FUNC_CALL_STAT& GetFuncCallStat(const INVOC_INFO& call)
	{
		func2vect_t::const_iterator i = func2vect.find(call.address);
		ASSERT(i != func2vect.end());
		ASSERT(call.invocation <= (int)i->second.size());
		return i->second[call.invocation-1];
	}

private:
	void AddToCache(const INVOC_INFO& call, stat_val_t value) const {cache[id][call] = value;}

private:
	mutable int id;
	static cache_t cache;
}; // class CWinProfStatistics