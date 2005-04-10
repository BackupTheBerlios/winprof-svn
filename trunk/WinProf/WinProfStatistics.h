#pragma once

#pragma warning( disable : 4996 )

#include "FunctionTreeView.h"
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
	DWORD address;
	INT invocation;
	DWORD64 runtime;
}; // class INVOC_INFO

class CWinProfStatistics;

typedef vector<FUNC_CALL_STAT*> calls_vector_t;
typedef hash_map<DWORD/*func address*/, calls_vector_t*> func2vect_t;
typedef hash_map<CString, CWinProfStatistics*> statistics_t;

class CWinProfStatistics 
{
public:
	CWinProfStatistics(void) {}
	virtual ~CWinProfStatistics(void) {}

	virtual CString GetString(const INVOC_INFO& call) const = 0;
	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const = 0;
	virtual CString GetStatName(void) const = 0;
}; // class CWinProfStatistics

template <typename T> 
int StatCompare(const T &t1, const T &t2) {
	if (t1 == t2) return 0;
	return (t1 < t2) ? -1 : 1;
}
