#pragma once

#include "Filter.h"

class CCompositeFilter : public CFilter
{
public:
	CCompositeFilter(const CFilter* v1, const CFilter* v2, logical_oper op)
		: f1(v1)
		, f2(v2)
		, oper(op)
	{}
	virtual ~CCompositeFilter(void) {}

	virtual bool IsDependandOn(CString n) const
	{
		bool dep = f1->IsDependandOn(n);
		if (f2 != NULL) dep = dep || f2->IsDependandOn(n);
		return dep;
	}

	// make use of a filter
	virtual bool Satisfies(const INVOC_INFO& iv) const
	{
		return oper(f1, f2, iv);
	}

private:
	const CFilter* f1;
	const CFilter* f2;
	logical_oper oper;
};