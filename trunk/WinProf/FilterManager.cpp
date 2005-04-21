#include "stdafx.h"
#include "FilterManager.h"

void CFilterManager::BuildFilters(CString expr)
{
}

void CFilterManager::DestroyFilters()
{
	delete filter;
	filter = NULL;
}