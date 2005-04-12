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

INT CStatManager::UpdateStatsWith(INVOC_INFO& call)
{
	ASSERT(call.invocation == 0); // the data comes from FillTree and is not known by this time
	ASSERT(call.runtime == 0); // the run time is also not known
	// call.invocation is updated inside the call
	GetNextEntry(call) = new FUNC_CALL_STAT(call.runtime);
	return call.invocation; // the returned value is already not 0
}

void CStatManager::UpdateRunTime(INVOC_INFO& call) 
{
	//TRACE("updating the run time to value: %s \n", CFunctionTreeView::dword64tostr(call.runtime));
	FUNC_CALL_STAT* func_call = GetExistingEntry(call);
	if (func_call == NULL) {
		//TRACE("the data is not found by address %lu\n", call.address);
		return;
	}
	func_call->runtime = call.runtime; // updated
}

FUNC_CALL_STAT* CStatManager::GetExistingEntry(const INVOC_INFO& call)
{
	//TRACE("find and existing entry, call invoc: %d \n", call.invocation);

	func2vect_t::const_iterator it = func2vect.find(call.address);
	if(it == func2vect.end()) {
		//TRACE("%lu hash entry not found \n", call.address);
		return NULL;
	}

	calls_vector_t* calls = it->second;
	int pos = call.invocation - 1;
	if (pos < 0 || pos >= (int)calls->size()) {
		//TRACE("for %lu: pos = %d, size = %d \n", call.address, pos, (int)calls->size());
		return NULL;
	}

	return (*calls)[pos];
}

FUNC_CALL_STAT*& CStatManager::GetNextEntry(INVOC_INFO& call)
{
	func2vect_t::const_iterator it = func2vect.find(call.address);

	calls_vector_t* calls;
	if(it == func2vect.end()) { 
		//TRACE("created entry for address: %lu \n", call.address);
		func2vect[call.address] = new calls_vector_t;
		calls = func2vect[call.address];
	}
	else
		calls = it->second;

	calls->push_back(NULL);
	call.invocation = (int)calls->size();
	return calls->back();
}
