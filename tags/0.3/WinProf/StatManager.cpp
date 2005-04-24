#include "StdAfx.h"
#include "StatManager.h"
#include "WinProfStatistics.h"
#include "RunTimeStat.h"
#include "NetRunTimeStat.h"
#include "CallDepthStat.h"
#include "TotalTimeStat.h"
#include "CountCallsStat.h"
#include "AvgTimeStat.h"
#include "MinMaxTimeStat.h"
#include <hash_map>
#include <vector>

using namespace std;

CStatManager::CStatManager(void)
{
	AddStatClass(new CTotalTimeStat);
	AddStatClass(new CTotalSquaredTimeStat);
	AddStatClass(new CRunTimeStat);
	AddStatClass(new CNetRunTimeStat);
	AddStatClass(new CCallDepthStat);
	AddStatClass(new CCountCallsStat);
	AddStatClass(new CMinTimeStat);
	AddStatClass(new CAvgTimeStat);
	AddStatClass(new CMaxTimeStat);
	AddStatClass(new CStdDevTimeStat);
}

CStatManager::~CStatManager(void)
{
	for (statistics_t::const_iterator i = stats.begin(); i != stats.end(); ++i)
		delete *i;
}

void CStatManager::AddStatClass(const CWinProfStatistics* stat)
{
	int id = (int)stats.size();
	stname2index[stat->GetStatName()] = id;
	stat->SetId(id);
	stats.push_back(stat);
}

void CStatManager::Clear(void)
{
	func2vect.clear();
	CWinProfStatistics::ClearCache();
}

INT CStatManager::UpdateStatsWith(const INVOC_INFO& call)
{
	// the data comes from FillTree and is not known by this time of call
	ASSERT(call.invocation == -1); 
	calls_vector_t& vec = func2vect[call.address];
	vec.push_back(FUNC_CALL_STAT());
	return (int)vec.size();
}

void CStatManager::UpdateRunTime(const INVOC_INFO& call, HTREEITEM item, DWORD64 time) 
{
	func2vect_t::iterator it = func2vect.find(call.address);
	ASSERT(it != func2vect.end());

	calls_vector_t& calls = it->second;
	int pos = call.invocation - 1;
	ASSERT(pos >= 0 && pos < (int)calls.size());

	calls[pos].treeitem = item;
	calls[pos].runtime = time;
}

func2vect_t CStatManager::func2vect;
statistics_t CStatManager::stats;
stname2index_t CStatManager::stname2index;
cache_t CWinProfStatistics::cache;

const func2vect_t& CWinProfStatistics::func2vect(CStatManager::GetDataBaseRef());
const statistics_t& CWinProfStatistics::stats(CStatManager::GetStats());
