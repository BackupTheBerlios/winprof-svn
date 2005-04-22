#pragma once

#include <vector>
#include <hash_map>
#include <string>

class CSymbolManager;
class CStatManager;

class INVOC_INFO {
public:
	explicit INVOC_INFO(DWORD addr, INT invoc=-1) : address(addr), invocation(invoc) {}
	operator size_t() const {return address ^ invocation;}
	bool operator <(const INVOC_INFO& x) const
		{return (address < x.address) || (address == x.address && invocation < x.invocation);}
	DWORD address;
	INT invocation;
}; // class INVOC_INFO

class LIST_ITEM_DATA
{
public:
	int line;
	INVOC_INFO invoc_info;
	LIST_ITEM_DATA(int l, INVOC_INFO ii) : line(l), invoc_info(ii) {}

	static CSymbolManager* man;
	static CStatManager* stats;
}; // class LIST_ITEM_DATA

class FUNC_CALL_STAT {
public:
	FUNC_CALL_STAT(HTREEITEM item=NULL, DWORD64 time=0) : treeitem(item), runtime(time) {}
	HTREEITEM treeitem;
	DWORD64 runtime;
}; // class FUNC_CALL_STAT

typedef union {
	DWORD64 dw64_val;
	int int_val;
	double double_val;
} stat_val_t;

class CWinProfStatistics;
typedef std::vector<FUNC_CALL_STAT> calls_vector_t;
typedef stdext::hash_map<DWORD/*address*/, calls_vector_t> func2vect_t;
typedef std::vector<const CWinProfStatistics*> statistics_t;
typedef stdext::hash_map<INVOC_INFO, stat_val_t> stat_cache_t;
typedef std::vector<stat_cache_t> cache_t;
typedef stdext::hash_map<std::string, int> stname2index_t;

class CFilter;
typedef bool(*logical_oper)(const CFilter*, const CFilter*, const INVOC_INFO&);

namespace CmpOper 
{
	typedef enum{LEQ=0, LES, GEQ, GRT, EQV, NEQ, CMP_OPER_COUNT} cmp_oper;
	extern const char* caption[CMP_OPER_COUNT];

	template<typename T> bool Leq(const T& v1, const T& v2) {return v1 <= v2;}
	template<typename T> bool Les(const T& v1, const T& v2) {return v1 < v2;}
	template<typename T> bool Geq(const T& v1, const T& v2)	{return v1 >= v2;}
	template<typename T> bool Grt(const T& v1, const T& v2) {return v1 > v2;}
	template<typename T> bool Eqv(const T& v1, const T& v2)	{return v1 == v2;}
	template<typename T> bool Neq(const T& v1, const T& v2)	{return v1 != v2;}

	typedef bool(*cmp_oper_func_double)(const double&, const double&);
	typedef bool(*cmp_oper_func_dw64)(const DWORD64&, const DWORD64&);
	typedef bool(*cmp_oper_func_int)(const int&, const int&);

	extern cmp_oper_func_double cmp_oper_vect_double[CMP_OPER_COUNT];
	extern cmp_oper_func_int cmp_oper_vect_int[CMP_OPER_COUNT];
	extern cmp_oper_func_dw64 cmp_oper_vect_dw64[CMP_OPER_COUNT];
}; // namespace CmpOper 

typedef stdext::hash_map<std::string, CFilter*> filname2fil_t;
typedef std::vector<const INVOC_INFO*> filtered_list_t;
