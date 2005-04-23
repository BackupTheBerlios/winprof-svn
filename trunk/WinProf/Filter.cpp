#include "StdAfx.h"
#include "Filter.h"
#include "AtomFilter.h"
#include "CompositeFilter.h"

CFilter* CFilter::CreateNew(CString expr, CString n, const CFilter* f, logical_oper oper)
{
	return (new CCompositeFilter(expr, n, this, f, oper));
}

const statistics_t& CAtomFilter::stats = CStatManager::GetStats();
