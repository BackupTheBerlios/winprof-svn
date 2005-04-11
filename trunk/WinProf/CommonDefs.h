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

typedef FUNC_CALL_STAT* FUNC_CALL_STAT_PTR;

class INVOC_INFO {
public:
	INVOC_INFO(DWORD a, INT i, DWORD64 r) 
		: address(a), invocation(i), runtime(r) {}
public:
	DWORD address;
	INT invocation;
	DWORD64 runtime;
}; // class INVOC_INFO

class CWinProfStatistics;

typedef vector<FUNC_CALL_STAT*> calls_vector_t;
typedef hash_map<DWORD/*func address*/, calls_vector_t*> func2vect_t;
typedef hash_map<CString, CWinProfStatistics*> statistics_t;
