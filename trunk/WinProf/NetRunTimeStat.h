#pragma once

#include "RunTimeStat.h"

class CNetRunTimeStat : public CWinProfStatistics
{
public:
	virtual CString GetString(const INVOC_INFO& call) const
		{return DWORD64ToStr(GetStatValue(call).dw64_val * 1000 / CWinProfDoc::m_Frequency);}
	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const
		{return CWinProfStatistics::StatCompare<DWORD64>(GetStatValue(c1).dw64_val, GetStatValue(c2).dw64_val);}
	virtual CString GetStatName(void) const 
		{return "Net Run Time (ms)";}
	virtual bool IsPerInvocation(void) const
		{return true;}

protected:
	virtual stat_val_t CalculateStatVal(const calls_vector_t& v, const INVOC_INFO& call) const
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
		stat_val_t val;
		val.dw64_val = time;
		return val;
	}
};
