#pragma once

#include "Filter.h"
#include "CommonDefs.h"

class LogicalOper {
public:

	static bool NOT(const CFilter* f1, const CFilter* f2, const INVOC_INFO& iv)
		{return !f1->FilterOut(iv);}

	static bool AND(const CFilter* f1, const CFilter* f2, const INVOC_INFO& iv)
		{return f1->FilterOut(iv) && f2->FilterOut(iv);}

	static bool OR(const CFilter* f1, const CFilter* f2, const INVOC_INFO& iv)
		{return f1->FilterOut(iv) || f2->FilterOut(iv);}
};