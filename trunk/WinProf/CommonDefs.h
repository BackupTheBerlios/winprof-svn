#pragma once
#pragma warning( disable : 4996 )

#include <vector>
#include <hash_map>

using namespace std;

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
	FUNC_CALL_STAT(DWORD64 time) : runtime(time) {}
	DWORD64 runtime;
}; // class FUNC_CALL_STAT

class INVOC_INFO {
public:
	INVOC_INFO(DWORD a, INT i) : address(a), invocation(i) {}
	DWORD address;
	INT invocation;
}; // class INVOC_INFO

typedef enum {TOTAL_TIME=0, COUNT_CALLS, AVG_TIME} stats_type;

class CWinProfStatistics;
typedef vector<FUNC_CALL_STAT*> calls_vector_t;
typedef hash_map<DWORD/*func address*/, calls_vector_t*> func2vect_t;
typedef hash_map<stats_type, CWinProfStatistics*> statistics_t;