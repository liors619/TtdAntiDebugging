#include "pch.h"
#include <tchar.h>
#include <easyhook.h>
#include <string>
#include <iostream>
#include <Windows.h>
#include <WinNT.h>
#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <string>



DWORD gFreqOffset = 0;
NTSTATUS Process32NextHook(
	HANDLE hSnapshot,
	LPPROCESSENTRY32 lppe
)
{
	printf("blabla\n");
	Process32Next(hSnapshot, lppe);
	return FALSE;
}

// EasyHook will be looking for this export to support DLL injection. If not found then 
// DLL injection will fail.
extern "C" void __declspec(dllexport) __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO * inRemoteInfo);

void __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO* inRemoteInfo)
{

	// Perform hooking
	HOOK_TRACE_INFO hHook = { NULL }; // keep track of our hook

	// Install the hook
	NTSTATUS result = LhInstallHook(
		GetProcAddress(GetModuleHandle(TEXT("Kernel32.dll")), "Process32Next"),
		Process32Next,
		NULL,
		&hHook);
	if (FAILED(result))
	{
		MessageBox(GetActiveWindow(), (LPCWSTR)RtlGetLastErrorString(), (LPCWSTR)L"Failed to install hook", MB_OK);
	}

	// If the threadId in the ACL is set to 0,
	// then internally EasyHook uses GetCurrentThreadId()
	ULONG ACLEntries[1] = { 0 };

	// Disable the hook for the provided threadIds, enable for all others
	LhSetExclusiveACL(ACLEntries, 1, &hHook);

	return;
}