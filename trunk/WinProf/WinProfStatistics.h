#pragma once

#include "StatManager.h"

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
	CWinProfStatistics(void) {}
	virtual ~CWinProfStatistics(void) {}

	// returns the statistics value as a string
	virtual CString GetString(const INVOC_INFO& call) const {return CString();}
	// sorting_by support
	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const {return 0;}
	// returns the column name, as it appears to user in GUI
	virtual CString GetStatName(void) const {return CString();}
	// width of column, as it appears to user in GUI
	virtual int GetWidth(void) const {return 100;}
	// where in column the text is placed
	virtual int GetColumnPlace(void) const {return LVCFMT_LEFT;}
	// defines stats type
	virtual bool IsPerInvocation(void) const {return false;}
	virtual bool IsPerFunciton(void) const {return false;}
	// clears the cache if at all
	virtual void ClearCache(void) const {}
	// filters support
	virtual bool Satisfies(const INVOC_INFO& iv, stat_val_t bound, cmp_oper oper) const {return false;}
	// used when user enters a string as a bound value
	virtual stat_val_t GetNumerical(const CString& str) {stat_val_t val; return val;}
	
protected:
	// if true is returned, then val is properly updated
	virtual bool SearchCache(const INVOC_INFO& call, stat_val_t& val) const {return false;}
	// calculate stat value
	virtual stat_val_t CalculateStatVal(const calls_vector_t& v, const INVOC_INFO& call) const = 0;

public:
	virtual stat_val_t GetStatValue(const INVOC_INFO& call) const
	{
		stat_val_t value;
		if (SearchCache(call, value)) return value;
		func2vect_t::const_iterator it = func2vect.find(call.address);
		ASSERT(it != func2vect.end());
		if (it == func2vect.end()) return value; // should never happen
		return CalculateStatVal(it->second, call); // cache updated inside if needed
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
}; // class CWinProfStatistics