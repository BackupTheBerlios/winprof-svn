#pragma once

#include "StdAfx.h"
#include "CommonDefs.h"

using namespace std;

class CStatManager
{
public:
	CStatManager(void);
	~CStatManager(void);

	// make these functions non static to ensure that the object is created
	// if so, then all the allocated data is deleted "automatically" in destructor

	// returns the invocation number
	INT UpdateStatsWith(const INVOC_INFO& call);
	void UpdateRunTime(const INVOC_INFO& call, DWORD64 time); // workaround

private: // auxiliary functions
	static FUNC_CALL_STAT* GetCall(const INVOC_INFO& call);
	static FUNC_CALL_STAT*& AddCall(const INVOC_INFO& call, int& invoc);

public:
	static statistics_t& GetStats(void) {return stats;}
	static func2vect_t& GetDataBaseRef(void) {return func2vect;}

private:
	static func2vect_t func2vect;
	static statistics_t stats;
}; // class CStatManager