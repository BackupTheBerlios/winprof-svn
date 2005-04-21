#pragma once

#include "StatManager.h"
#include <string>

using namespace std;
/*template<typename T> 
class CCmpEnv {
public:
	enum{SIZE = 5};
	static bool Leq(T v1, T v2) {return v1 <= v2;}
	static bool Les(T v1, T v2) {return v1 < v2;}
	static bool Geq(T v1, T v2) {return v1 >= v2;}
	static bool Grt(T v1, T v2) {return v1 > v2;}
	static bool Eqv(T v1, T v2) {return v1 == v2;}

	typedef bool(*cmp_func)(T, T);
	
	static cmp_func cmp_func_vect[SIZE];
}; // class CmpEnv

template<typename T> 
static CCmpEnv<T>::cmp_func CCmpEnv<T>::cmp_func_vect = {
	CCmpEnv<T>::Leq, 
	CCmpEnv<T>::Les, 
	CCmpEnv<T>::Geq, 
	CCmpEnv<T>::Grt, 
	CCmpEnv<T>::Eqv
};*/

class CWinProfStatistics 
{
public:
	CWinProfStatistics(void) {cache.push_back(stat_cache_t());}
	virtual ~CWinProfStatistics(void) {}

	// returns the statistics value as a string
	virtual CString GetString(const INVOC_INFO& call) const {return CString();}
	// sorting_by support
	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const {return 0;}
	// returns the column name, as it appears to user in GUI
	virtual CString GetStatCaption(void) const {return CString();}
	// returns stat name to be used by other stats
	virtual string GetStatName(void) const = 0;
	// width of column, as it appears to user in GUI
	virtual int GetWidth(void) const {return 100;}
	// where in column the text is placed
	virtual int GetColumnPlace(void) const {return LVCFMT_LEFT;}
	// defines stats type
	virtual bool IsPerInvocation(void) const {return false;}
	virtual bool IsPerFunction(void) const {return false;}
	// filters support
	virtual bool Satisfies(const INVOC_INFO& iv, stat_val_t bound, cmp_oper oper) const {return false;}
	// used when user enters a string as a bound value
	virtual stat_val_t GetNumerical(CString str) const {return stat_val_t();}
	// does the statistic use cache
	virtual bool IsCacheable() const {return false;}
	
protected:
	// calculate stat value
	virtual stat_val_t CalculateStatVal(const calls_vector_t& v, const INVOC_INFO& call) const = 0;

public:
	stat_val_t GetStatValue(const INVOC_INFO& call) const
	{
		stat_val_t value;
		if (IsCacheable() && SearchCache(call, value)) return value;
		func2vect_t::const_iterator it = func2vect.find(call.address);
		ASSERT(it != func2vect.end());
		value = CalculateStatVal(it->second, call); // cache updated inside if needed
		if (IsCacheable()) AddToCache(call, value);
		return value;
	}

	void ClearCache(void) const {cache[id].clear();}
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

private:
	// clears the cache if at all
	void AddToCache(const INVOC_INFO& call, stat_val_t value) const {cache[id][call] = value;}

private:
	mutable int id;
	static cache_t cache;
}; // class CWinProfStatistics