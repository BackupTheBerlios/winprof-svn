#include "StdAfx.h"
#include "StatManager.h"

using namespace std;

CStatManager::CStatManager(void)
{
/*	CWinProfStatistics* p;
	p = new CCountCallsStat(func2vect); stats[p->GetStatName()] = p;
	p = new CTotalTimeStat(func2vect); stats[p->GetStatName()] = p;
	p = (CCountCallsStat*)new CAvgTimeStat(func2vect); stats[p->GetStatName()] = p; */
}

void CStatManager::UpdateStatsWith(const INVOC_INFO& call)
{
	func2vect_t::const_iterator it = func2vect.find(call.address);
	calls_vector_t* calls = NULL;
	if(it == func2vect.end()) {
		check_ptr(calls = new calls_vector_t);
		func2vect[call.address] = calls;
	} else {
		calls = func2vect[call.address];
	}
	size_t size = calls->size() + 1; // the invocation number
	ASSERT(call.invocation == size);

	FUNC_CALL_STAT* call_info = new FUNC_CALL_STAT(call.runtime);
	check_ptr(call_info);
	(*calls)[size] = call_info;
}

void CStatManager::check_ptr(void* ptr)
{
	if (ptr == NULL) {
		//MessageBox("cannot allocate memory");
		exit(1);
	}
}

const statistics_t& CStatManager::GetStats(void) const
{return stats;}
