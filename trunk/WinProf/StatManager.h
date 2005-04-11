#pragma once

#include "StdAfx.h"
#include "CommonDefs.h"

using namespace std;

class CStatManager
{
public:
	CStatManager(void);
	~CStatManager(void);

	// returns the invocation number
	INT UpdateStatsWith(INVOC_INFO& call);
	const statistics_t& GetStats(void) const {return stats;}
	static func2vect_t& GetDataBaseRef(void) {return func2vect;}

	// workaround
	void UpdateRunTime(INVOC_INFO& call);

	// auxiliary functions
	static FUNC_CALL_STAT* GetExistingEntry(const INVOC_INFO& call);
	static FUNC_CALL_STAT*& GetNextEntry(INVOC_INFO& call);

private:
	static func2vect_t func2vect;

private:
	static void check_ptr(void* ptr) {if (ptr == NULL) exit(1);}
	statistics_t stats;
}; // class CStatManager