#pragma once

#include "Filter.h"

class CFilterManager
{
	CFilterManager() : filter(NULL) {}
	void BuildFilters(CString expr);
	void DestroyFilters();
private:
	CFilter *filter;
};
