#include "StdAfx.h"
#include "StatManager.h"
#include "WinProfStatistics.h"
#include "FunctionStat.h"
#include "RunTimeStat.h"
#include "TotalTimeStat.h"
#include "CountCallsStat.h"
#include "AvgTimeStat.h"
#include <hash_map>
#include <vector>

using namespace std;

CStatManager::CStatManager(void)
{
	AddStatClass(new CRunTimeStat());
	AddStatClass(new CCountCallsStat());
	AddStatClass(new CTotalTimeStat());
	AddStatClass(static_cast<CCountCallsStat*>(new CAvgTimeStat()));
}

CStatManager::~CStatManager(void)
{
	for(int i = 0; i < (int)stats.size(); i++)
		delete stats[(stats_type)i];
}

void CStatManager::AddStatClass(const CWinProfStatistics* stat)
{
	stats[stat->GetStatID()] = stat;
}

void CStatManager::Clear(void)
{
	func2vect.clear();
	for (statistics_t::const_iterator iter = stats.begin(); iter != stats.end(); ++iter)
		iter->second->ClearCache();
}

INT CStatManager::UpdateStatsWith(const INVOC_INFO& call)
{
	// the data comes from FillTree and is not known by this time of call
	ASSERT(call.invocation == -1); 
	calls_vector_t& vec = func2vect[call.address];
	vec.push_back(FUNC_CALL_STAT());
	return (int)vec.size();
}

void CStatManager::UpdateRunTime(const INVOC_INFO& call, DWORD64 time) 
{
	func2vect_t::iterator it = func2vect.find(call.address);
	ASSERT(it != func2vect.end());

	calls_vector_t& calls = it->second;
	int pos = call.invocation - 1;
	ASSERT(pos >= 0 && pos < (int)calls.size());

	calls[pos].runtime = time;
}

func2vect_t CStatManager::func2vect;
statistics_t CStatManager::stats;

const func2vect_t& CWinProfStatistics::func2vect(CStatManager::GetDataBaseRef());

template <typename T, typename Function>
typename CFunctionStat<T, Function>::cache_map_t CFunctionStat<T, Function>::cache;
