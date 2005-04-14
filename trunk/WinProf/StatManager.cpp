#include "StdAfx.h"
#include "StatManager.h"
#include "TotalTimeStat.h"
#include "CountCallsStat.h"
#include "AvgTimeStat.h"
#include <hash_map>
#include <vector>

using namespace std;

func2vect_t CStatManager::func2vect;
statistics_t CStatManager::stats;

CStatManager::CStatManager(void)
{
	CWinProfStatistics* p;
	p = new CCountCallsStat(); stats[p->GetStatID()] = p;
	p = new CTotalTimeStat(); stats[p->GetStatID()] = p;
	p = (CCountCallsStat*)new CAvgTimeStat(); stats[p->GetStatID()] = p;
}

CStatManager::~CStatManager(void)
{
	func2vect_t::const_iterator hash_it = func2vect.begin();
	for(; hash_it != func2vect.end(); ++hash_it) {
		calls_vector_t* vect = hash_it->second;
		for(int i = 0; i < (int)vect->size(); i++) {
			delete (*vect)[i]; // delete the FUNC_CALL_STAT construction
		}
		// delete the vector
		delete vect;
	}
	for(int i = 0; i < (int)stats.size(); i++) {
		delete stats[(stats_type)i];
	}
}

INT CStatManager::UpdateStatsWith(const INVOC_INFO& call)
{
	// the data comes from FillTree and is not known by this time of call
	ASSERT(call.invocation == 0); 
	int invocation;
	AddCall(call, invocation) = new FUNC_CALL_STAT(0/*time - to be updated later*/);
	return invocation;
}

void CStatManager::UpdateRunTime(const INVOC_INFO& call, DWORD64 time) 
{
	FUNC_CALL_STAT* func_call = GetCall(call);
	if (func_call == NULL) return;
	func_call->runtime = time;
}

FUNC_CALL_STAT* CStatManager::GetCall(const INVOC_INFO& call)
{
	func2vect_t::const_iterator it = func2vect.find(call.address);
	if(it == func2vect.end()) return NULL;

	calls_vector_t* calls = it->second;
	int pos = call.invocation - 1;
	if (pos < 0 || pos >= (int)calls->size()) return NULL;

	return (*calls)[pos];
}

// in invoc the invocation number is returned
FUNC_CALL_STAT*& CStatManager::AddCall(const INVOC_INFO& call, int& invoc)
{
	func2vect_t::const_iterator it = func2vect.find(call.address);

	calls_vector_t* calls;
	if(it == func2vect.end()) { 
		calls = new calls_vector_t;
		func2vect[call.address] = calls;
	}
	else {
		calls = it->second;
	}

	calls->push_back(NULL);
	invoc = (int)calls->size();
	return calls->back();
}
