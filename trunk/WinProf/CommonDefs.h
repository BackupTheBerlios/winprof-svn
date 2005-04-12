#pragma once
#pragma warning( disable : 4996 )

#include <vector>
#include <hash_map>

using namespace std;

class FUNC_CALL_STAT {
public:
	FUNC_CALL_STAT(DWORD64 time) : runtime(time) {}
	DWORD64 runtime;
}; // class FUNC_CALL_STAT

class INVOC_INFO {
public:
	INVOC_INFO(DWORD a, INT i, DWORD64 r) 
		: address(a), invocation(i), runtime(r) {}
	DWORD address;
	INT invocation;
	DWORD64 runtime;
}; // class INVOC_INFO

typedef enum {TOTAL_TIME=0, COUNT_CALLS, AVG_TIME} stats_type;

class CWinProfStatistics;
typedef vector<FUNC_CALL_STAT*> calls_vector_t;
typedef hash_map<DWORD/*func address*/, calls_vector_t*> func2vect_t;
typedef hash_map<stats_type, CWinProfStatistics*> statistics_t;