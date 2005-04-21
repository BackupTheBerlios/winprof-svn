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

typedef enum{LEQ=0, LES, GEQ, GRT, EQV} cmp_oper;

