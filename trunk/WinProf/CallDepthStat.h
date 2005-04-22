#pragma once

#include "WinProfStatistics.h"

class CCallDepthStat : public CWinProfStatistics
{
public:
	virtual CString ToString(stat_val_t val) const
		{return Format("%d", val.int_val);}
	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const
		{return CWinProfStatistics::StatCompare<int>(GetStatValue(c1).int_val, GetStatValue(c2).int_val);}
	virtual CString GetStatCaption(void) const 
		{return "Call Depth";}
	virtual string GetStatName() const
		{return "CallDepthStat";}
	virtual bool IsPerInvocation(void) const
		{return true;}
	virtual bool Satisfies(const INVOC_INFO& iv, stat_val_t bound, cmp_oper oper) const
		{return CmpOper::cmp_oper_vect_int[oper](GetStatValue(iv).int_val, bound.int_val);}
	virtual stat_val_t GetNumerical(CString str) const
	{
		stat_val_t val;
		val.int_val = atoi(str);
		return val;
	}

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
