#include "StdAfx.h"
#include "Filter.h"
#include "AtomFilter.h"
#include "CompositeFilter.h"

CFilter* CFilter::CreateNew(const CFilter* f, logical_oper oper)
{
	return (new CCompositeFilter(this, f, oper));
}

class LogicalOper {
public:

	static bool NOT(const CFilter* f1, const CFilter* f2, const INVOC_INFO& iv)
		{return !f1->Satisfies(iv);}

	static bool AND(const CFilter* f1, const CFilter* f2, const INVOC_INFO& iv)
		{return f1->Satisfies(iv) && f2->Satisfies(iv);}

	static bool OR(const CFilter* f1, const CFilter* f2, const INVOC_INFO& iv)
		{return f1->Satisfies(iv) || f2->Satisfies(iv);}
};

const statistics_t& CAtomFilter::stats = CStatManager::GetStats();
