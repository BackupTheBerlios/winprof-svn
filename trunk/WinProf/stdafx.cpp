// stdafx.cpp : source file that includes just the standard includes
// WinProf++.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


CString DWORD64ToStr(DWORD64 x)
{
	if (x == 0) return "0";

	CString s;
	while (x)
	{
		s += (char)(x%10 + '0');
		x /= 10;
	}
	s.MakeReverse();
	return s;
}

CString DWORD64ToHexStr(DWORD64 x)
{
	if (x == 0) return "0";
	DWORD low = (DWORD)(x & ((1<<32)-1)), high = (DWORD)(x>>32);
	CString s;
	if (high) s.Format("%x", high);
	s.AppendFormat("%x", low);
	return s;
}
