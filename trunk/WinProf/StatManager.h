#pragma once
/*******************************************************************
* 1. when AvgTimeStat is created, then each one of TotalTime and CountCalls
*    is created an additional time (is created once in StatManager.cpp.
*    Can be resolved using references
********************************************************************/
#include "TotalTimeStat.h"
#include "CountCallsStat.h"
#include "AvgTimeStat.h"
#include <hash_map>
#include <vector>

using namespace std;

class CStatManager
{
public:
	CStatManager(void);
	~CStatManager(void);

	void UpdateStatsWith(const INVOC_INFO& call);
	const statistics_t& GetStats(void) const;

private:
	func2vect_t func2vect;

private:
	static void check_ptr(void* ptr);
	statistics_t stats;
}; // class CStatManager