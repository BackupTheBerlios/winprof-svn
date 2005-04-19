#include "StdAfx.h"
#include "Filter.h"
#include "ComplFilter.h"

CFilter* CFilter::CreateNew(const CFilter* f, logical_oper oper)
{
	return (new CComplFilter(this, f, oper));
}
