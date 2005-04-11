#include "StdAfx.h"
#include "StatManager.h"
#include "TotalTimeStat.h"
#include "CountCallsStat.h"
#include "AvgTimeStat.h"
#include <hash_map>
#include <vector>

using namespace std;

func2vect_t CStatManager::func2vect;

CStatManager::CStatManager(void)
{
/*	CWinProfStatistics* p;
	p = new CCountCallsStat(func2vect); stats[p->GetStatName()] = p;
	p = new CTotalTimeStat(func2vect); stats[p->GetStatName()] = p;
	p = (CCountCallsStat*)new CAvgTimeStat(func2vect); stats[p->GetStatName()] = p; */
}

CStatManager::~CStatManager(void)
{
	func2vect_t::const_iterator hash_it = func2vect.begin();
	for(; hash_it != func2vect.end(); ++hash_it) {
		calls_vector_t* vect = hash_it->second;
		calls_vector_t::const_iterator vect_it = vect->begin();
		for(; vect_it != vect->end(); ++vect_it) {
			delete *vect_it; // delete the FUNC_CALL_STAT construction
		}
		// delete the vector
		delete vect;
	}
}

INT CStatManager::UpdateStatsWith(INVOC_INFO& call)
{
	ASSERT(call.invocation == 0); // the data comes from FillTree and is not known by this time
	ASSERT(call.runtime == 0); // the run time is also not known
	GetNextEntry(call) /* call.invocation is updated */ = new FUNC_CALL_STAT(call.runtime);
	return call.invocation;
}

void CStatManager::UpdateRunTime(INVOC_INFO& call) 
{
	FUNC_CALL_STAT* func_call = GetExistingEntry(call);
	if (!func_call) return;
	func_call->runtime = call.runtime; // updated
}

/****************************************
Don't try to change the data, just find 
the reference if it does exists.
*****************************************/
FUNC_CALL_STAT* CStatManager::GetExistingEntry(const INVOC_INFO& call)
{
	func2vect_t::const_iterator it = func2vect.find(call.address);
	if(it == func2vect.end())
		return NULL;

	calls_vector_t* calls = func2vect[call.address];
	if (call.invocation < 0 || call.invocation >= (INT)calls->size())
		return NULL;

	return (*calls)[call.invocation];
}

/****************************************
call.address [in]
	The function creates, if needed new hash entry for this address.
	In this case a new vector is also created.
	If the hash entry exists, then the vector is resized.
call.runtime [not used]
call.invocation [out]
	The number of current call is returned in this 
	(defined by the vector size)
ret val
	Reference to the existing or newly created entry,
	e.g. this is reference to pointer
*****************************************/
FUNC_CALL_STAT*& CStatManager::GetNextEntry(INVOC_INFO& call)
{
	func2vect_t::const_iterator it = func2vect.find(call.address);
	calls_vector_t* calls = NULL;
	
	if(it == func2vect.end()) { // the hash entry is not found : create it
		check_ptr(calls = new calls_vector_t);
		calls->resize(4);
		func2vect[call.address] = calls;
	} else { // the hash entry is found : used the existing value and resized if needed
		calls = func2vect[call.address];
		if (calls->size() == calls->max_size())
			calls->resize(calls->size() * 2);
	}

	call.invocation = (INT)calls->size() + 1; // the invocation number
	return (*calls)[call.invocation]; // returned reference to pointer
}
