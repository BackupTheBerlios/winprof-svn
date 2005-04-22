#pragma once

#include "RunTimeStat.h"

class CNetRunTimeStat : public CWinProfStatistics
{
public:
	virtual CString ToString(stat_val_t val) const
		{return DWORD64ToStr(val.dw64_val * 1000 / CWinProfDoc::m_Frequency);}
	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const
		{return CWinProfStatistics::StatCompare<DWORD64>(GetStatValue(c1).dw64_val, GetStatValue(c2).dw64_val);}
	virtual CString GetStatCaption(void) const 
		{return "Net Run Time (ms)";}
	virtual string GetStatName() const
		{return "NetRunTimeStat";}
	virtual bool IsPerInvocation(void) const
		{return true;}
	virtual bool IsCacheable() const 
		{return true;}
	virtual bool Satisfies(const INVOC_INFO& iv, stat_val_t bound, cmp_oper oper) const
		{return CmpOper::cmp_oper_vect_dw64[oper](GetStatValue(iv).dw64_val, bound.dw64_val);}
	virtual stat_val_t GetNumerical(CString str) const
	{
		stat_val_t val;
		val.dw64_val = _atoi64(str);
		return val;
	}

protected:
	virtual stat_val_t CalculateStatVal(const calls_vector_t& v, const INVOC_INFO& call) const
	{
		DWORD64 time = stats[CStatManager::GetStName2Index()["RunTimeStat"]]->GetStatValue(call).dw64_val;
		CTreeCtrl& tree = CStatManager::GetTreeCtrl();
		func2vect_t& func2vect = CStatManager::GetDataBaseRef();
		HTREEITEM item = v[call.invocation-1].treeitem;
		for (HTREEITEM child = tree.GetChildItem(item); child != NULL; child = tree.GetNextItem(child, TVGN_NEXT))
		{
			INVOC_INFO* ii = reinterpret_cast<INVOC_INFO*>(tree.GetItemData(child));
			time -= stats[CStatManager::GetStName2Index()["RunTimeStat"]]->GetStatValue(*ii).dw64_val;
		}
		stat_val_t val;
		val.dw64_val = time;
		return val;
	}
};
