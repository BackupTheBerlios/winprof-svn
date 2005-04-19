#pragma once

#include "CommonDefs.h"

class CFilter
{
public:
	CFilter(void) {}
	virtual ~CFilter(void) {}

	// creation of new objects, dynamic allocation used
	CFilter* CreateNew(const CFilter* f, logical_oper oper);

	// make use of a filter
	virtual bool FilterOut(const INVOC_INFO& iv) const = 0;
};