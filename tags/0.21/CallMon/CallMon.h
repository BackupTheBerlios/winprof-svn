#ifndef CALLMON_H
#define CALLMON_H

#include <windows.h>

enum {CALL_INFO_START, CALL_INFO_END};

typedef struct
{
	DWORD type;
	DWORD address;
	DWORD64 time;
} CALL_INFO;

#endif
