#include "CallMon.h"
#include <vector>

#define OFFSET_CALL 5				// 1 byte for "call" instruction + 4 bytes for address

struct RETURN_ADDRESS
{
	DWORD address;
	DWORD return_address;
};

static BOOL FullWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite)
{
	DWORD written;
	do
	{
		if (!WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, &written, NULL))
			return FALSE;
		nNumberOfBytesToWrite -= written;
		lpBuffer = (LPCVOID)((LPCSTR)lpBuffer + written);
	} while (nNumberOfBytesToWrite > 0);
	return TRUE;
}

/*
we predict that there are less than 50 nested function calls
in this case the vector won't reallocate the objects, so performance won't suffer
*/
static std::vector<RETURN_ADDRESS> return_addr;
static std::vector<CALL_INFO> call_info;


static void LogToFile()
{
	static TCHAR szTempPath[MAX_PATH];
	static bool FirstTime = true;
	HANDLE hFile;
	if (FirstTime)
	{
		TCHAR temp[MAX_PATH];
		GetTempPath(MAX_PATH, temp);
		wsprintf(szTempPath, "%scalllog.%d", temp, GetCurrentProcessId());
		FirstTime = false;
		hFile = CreateFile(szTempPath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	}
	else
		hFile = CreateFile(szTempPath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, 0, NULL);
	if (hFile == NULL)
	{
		MessageBox(NULL, "Couldn't open file calllog.prof", "WinProf++", MB_ICONERROR | MB_OK);
		ExitProcess(1);
	}
	SetFilePointer(hFile, 0, NULL, FILE_END);
	for (std::vector<CALL_INFO>::const_iterator iter = call_info.begin(); iter != call_info.end(); ++iter)
	{
		if (!FullWriteFile(hFile, &*iter, sizeof(CALL_INFO)))
		{
			MessageBox(NULL, "Couldn't write to file calllog.prof", "WinProf++", MB_ICONERROR | MB_OK);
			ExitProcess(1);
		}
	}
	call_info.clear();
	CloseHandle(hFile);
}

//type cast : conversion from 'DWORD' to 'unsigned long *' of greater size
#pragma warning(push)
#pragma warning(disable : 4311)
#pragma warning(disable : 4312)

extern "C" static __declspec(naked) void _pexit()
{
	// prolog
	__asm
	{
		push 0						// placeholder for return address
		push ebp					// setup standard stack frame
		mov ebp, esp
		sub esp, __LOCAL_SIZE		// leave space for local variables
		push eax					// save original function return value
		push edx
		push esi					// VC++ uses esi to protect esp
	}

    DWORD frame_ptr;
	LARGE_INTEGER time;
	CALL_INFO ci;
	QueryPerformanceCounter(&time);
	ci.type = CALL_INFO_END;
	ci.address = return_addr.back().address;
	ci.time = time.QuadPart;
	call_info.push_back(ci);
    
	__asm mov DWORD PTR [frame_ptr], ebp
	*(unsigned long *)(frame_ptr + 4) = return_addr.back().return_address;
	return_addr.pop_back();

	LogToFile();

	
	// epilog
	__asm
	{
		pop esi
		pop edx						// restore registers
		pop eax
 		mov esp, ebp				// restore the standard stack frame
		pop ebp
		ret
	}
}

extern "C" __declspec(naked) void _penter()
{
	// prolog
	__asm
	{
		push ebp					// setup standard stack frame
		mov ebp, esp
		sub esp, __LOCAL_SIZE		// leave space for local variables
		pushad						// save all registers
	}

    DWORD frame_ptr;
	LARGE_INTEGER time;
	CALL_INFO *ci;
	RETURN_ADDRESS ra;
	__asm mov DWORD PTR [frame_ptr], ebp

	call_info.push_back(CALL_INFO());
	ci = &call_info.back();
	ci->address = *(unsigned long *)(frame_ptr + 4) - OFFSET_CALL;	// of called function
	ra.address = ci->address;
	ra.return_address = *(unsigned long *)(frame_ptr + 8);			// where the function returns to
	*(unsigned long *)(frame_ptr + 8) = (unsigned long)_pexit;
	return_addr.push_back(ra);
	ci->type = CALL_INFO_START;
	QueryPerformanceCounter(&time);
	ci->time = time.QuadPart;

	// epilog
	__asm
	{
		popad						// restore registers
 		mov esp, ebp				// restore the standard stack frame
		pop ebp
		ret
	}
}

#pragma warning(pop)
