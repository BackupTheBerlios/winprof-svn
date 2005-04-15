#pragma once

#include <vector>
#include <hash_map>

class CSymbolManager;
class CStatManager;

// defined here to be initialized
class LIST_ITEM_DATA
{
public:
	int line;
	DWORD address;
	LIST_ITEM_DATA(int l, DWORD a) : line(l), address(a) {}

	static CSymbolManager* man;
	static CStatManager* stats;
}; // class LIST_ITEM_DATA

class FUNC_CALL_STAT {
public:
	FUNC_CALL_STAT(DWORD64 time=0) : runtime(time) {}
	DWORD64 runtime;
}; // class FUNC_CALL_STAT

class INVOC_INFO {
public:
	explicit INVOC_INFO(DWORD addr, INT invoc=-1) : address(addr), invocation(invoc) {}
	bool operator< (const INVOC_INFO &x) const
		{return (address < x.address) || (address == x.address && invocation < x.invocation);}
		operator size_t() const {return address ^ invocation;}
	DWORD address;
	INT invocation;
}; // class INVOC_INFO

typedef enum {RUN_TIME, TOTAL_TIME, COUNT_CALLS, AVG_TIME} stats_type;

class CWinProfStatistics;
typedef std::vector<FUNC_CALL_STAT> calls_vector_t;
typedef stdext::hash_map<DWORD/*address*/, calls_vector_t> func2vect_t;
typedef stdext::hash_map<stats_type, const CWinProfStatistics*> statistics_t;