#pragma once

#include "WinProfStatistics.h"

class CCallDepthStat : public CWinProfStatistics
{
public:
	virtual CString GetString(const INVOC_INFO& call) const
		{return Format("%d", GetStatValue(call).int_val);}
	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const
		{return CWinProfStatistics::StatCompare<int>(GetStatValue(c1).int_val, GetStatValue(c2).int_val);}
	virtual CString GetStatName(void) const 
		{return "Call Depth";}
	virtual bool IsPerInvocation(void) const
		{return true;}

protected:
	virtual stat_val_t CalculateStatVal(const calls_vector_t& v, const INVOC_INFO& call) const
	{
		CTreeCtrl& tree = CStatManager::GetTreeCtrl();
		HTREEITEM root = tree.GetRootItem();
		int depth = 0;
		for (HTREEITEM item = v[call.invocation-1].treeitem; item != root; item = tree.GetParentItem(item))
			depth++;
		stat_val_t val;
		val.int_val = depth;
		return val;
	}
	
};
