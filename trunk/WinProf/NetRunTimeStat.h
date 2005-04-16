#pragma once

#include "FunctionStat.h"
#include "RunTimeStat.h"

class CStatHelperNetRunTime
{
public:
	DWORD64 operator()(const calls_vector_t& v, const INVOC_INFO& call)
	{
		DWORD64 time = v[call.invocation-1].runtime;
		CTreeCtrl& tree = CStatManager::GetTreeCtrl();
		func2vect_t& func2vect = CStatManager::GetDataBaseRef();
		HTREEITEM item = v[call.invocation-1].treeitem;
		for (HTREEITEM child = tree.GetChildItem(item); child != NULL; child = tree.GetNextItem(child, TVGN_NEXT))
		{
			INVOC_INFO* ii = reinterpret_cast<INVOC_INFO*>(tree.GetItemData(child));
			time -= func2vect[ii->address][ii->invocation-1].runtime;
		}
		return time;
	}
};

class CNetRunTimeStat : public CFunctionStat<DWORD64, CStatHelperNetRunTime>
{
public:
	CNetRunTimeStat(void) : CFunctionStat<DWORD64, CStatHelperNetRunTime>(CStatHelperNetRunTime()) {}

	virtual CString GetString(const INVOC_INFO& call) const
		{return DWORD64ToStr(CFunctionStat<DWORD64, CStatHelperNetRunTime>::GetStatValue(call) * 1000 / CWinProfDoc::m_Frequency);}

	virtual CString GetStatName(void) const 
		{return "Net Run Time (ms)";}

	virtual bool IsPerInvocation(void) const
		{return true;}
};
