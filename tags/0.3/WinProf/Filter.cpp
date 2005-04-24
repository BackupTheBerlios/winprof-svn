#include "StdAfx.h"
#include "Filter.h"
#include "AtomFilter.h"
#include "CompositeFilter.h"

CFilter* CFilter::CreateNew(CString expr, CString n, CString son, logical_oper oper)
{
	return (new CCompositeFilter(expr, n/*the new name*/, name/*first son*/, son/*second son*/, oper));
}

const statistics_t& CAtomFilter::stats = CStatManager::GetStats();
