#pragma once

#include "FunctionStat.h"

class CStatHelperCallDepth
{
public:
	int operator() (const calls_vector_t& v, const INVOC_INFO& call)
	{
		CTreeCtrl& tree = CStatManager::GetTreeCtrl();
		HTREEITEM root = tree.GetRootItem();
		int depth = 0;
		for (HTREEITEM item = v[call.invocation-1].treeitem; item != root; item = tree.GetParentItem(item))
			depth++;
		return depth;
	}
};

class CCallDepthStat : public CFunctionStat<int, CStatHelperCallDepth>
{
public:
	CCallDepthStat(void) : CFunctionStat<int, CStatHelperCallDepth>(CStatHelperCallDepth()) {}

	virtual CString GetString(const INVOC_INFO& call) const
		{return Format("%d", GetStatValue(call));}

	virtual CString GetStatName(void) const 
		{return "Call Depth";}

	virtual bool IsPerInvocation(void) const
		{return true;}
};
