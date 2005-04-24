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

CString Format(LPCTSTR format, ...)
{
	va_list args;
	CString s;
	va_start(args, format);
	s.FormatV(format, args);
	va_end(args);
	return s;
}

bool ValidForStat(CString str,  bool(*good)(char c))
{
	int len = str.GetLength();
	if (len == 0) return false;
	for(int i = 0; i < len; ++i) {
		if (!good(str[i])) return false;
	}
	return true;
}

bool IsDigit(char c) {return ('0' <= c && c <= '9');}
bool IsDigitDot(char c) {return (('0' <= c && c <= '9') || c == '.');}
bool IsValidName(char c) 
{
	return (
			('a' <= c && c <= 'z') || 
			('A' <= c && c <= 'Z') || 
			('0' <= c && c <= '9') || 
			(c == '_')
	);
}
