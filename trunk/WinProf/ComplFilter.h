#pragma once

#include "Filter.h"
#include "LogicalOper.h"

class CComplFilter : public CFilter
{
public:
	CComplFilter(const CFilter* v1, const CFilter* v2, logical_oper op)
		: f1(v1)
		, f2(v2)
		, oper(op)
	{}
	virtual ~CComplFilter(void) {}

	// make use of a filter
	virtual bool FilterOut(const INVOC_INFO& iv) const
	{
		return oper(f1, f2, iv);
	}

private:
	const CFilter* f1;
	const CFilter* f2;
	logical_oper oper;
};