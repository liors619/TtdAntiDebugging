/*

// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

#include <Windows.h>
#include <detours.h>
#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <TlHelp32.h>
    
//int (WINAPI* Real_Send)(SOCKET s, const char* buf, int len, int flags) = send;
//int (WINAPI* Real_Recv)(SOCKET s, char* buf, int len, int flags) = recv;
//int WINAPI Mine_Send(SOCKET s, const char* buf, int len, int flags);
//int WINAPI Mine_Recv(SOCKET s, char* buf, int len, int flags);

static BOOL (WINAPI* RealProcessNext)(HANDLE hSnapshot, LPPROCESSENTRY32 lppe) = Process32Next;

using namespace std;
BOOL WINAPI OurProcess32Next(HANDLE hSnapshot, LPPROCESSENTRY32 lppe) {
    cout << "yo" << endl;
    return TRUE;
}


BOOL WINAPI DllMain(HINSTANCE, DWORD dwReason, LPVOID) {
    cout << "version 1.01" << endl;
    cout << dwReason << endl;

    if (DetourIsHelperProcess()) {
        return TRUE;
    }

    if (dwReason == DLL_PROCESS_ATTACH) {
        DetourRestoreAfterWith();

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)RealProcessNext, OurProcess32Next);
        DetourTransactionCommit();
    }
    else if (dwReason == DLL_PROCESS_DETACH) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)RealProcessNext, OurProcess32Next);
        DetourTransactionCommit();
    }
    return TRUE;
}
*/
#include "pch.h"


#include <detours.h>
#include <string>
#include <TlHelp32.h>

static LONG dwSlept = 0;

// Target pointer for the uninstrumented Sleep API.
//
static VOID(WINAPI* TrueSleep)(DWORD dwMilliseconds) = Sleep;
static BOOL(WINAPI* RealProcessNext)(HANDLE hSnapshot, LPPROCESSENTRY32 lppe) = Process32NextW;

// Detour function that replaces the Sleep API.
//
VOID WINAPI TimedSleep(DWORD dwMilliseconds)
{
    printf("our sleep :)\n");
    // Save the before and after times around calling the Sleep API.
    DWORD dwBeg = GetTickCount();
    TrueSleep(dwMilliseconds);
    DWORD dwEnd = GetTickCount();

    InterlockedExchangeAdd(&dwSlept, dwEnd - dwBeg);
}


BOOL WINAPI OurProcess32Next(HANDLE hSnapshot, LPPROCESSENTRY32 lppe) {
    printf("our processNext :)\n");
    return Process32NextW(hSnapshot, lppe);
}

// DllMain function attaches and detaches the TimedSleep detour to the
// Sleep target function.  The Sleep target function is referred to
// through the TrueSleep target pointer.

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved)
{
    printf("inject version 1.02\n");
    if (DetourIsHelperProcess()) {
        return TRUE;
    }

    if (dwReason == DLL_PROCESS_ATTACH) {
        DetourRestoreAfterWith();

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)RealProcessNext, OurProcess32Next);
        DetourAttach(&(PVOID&)TrueSleep, TimedSleep);
        DetourTransactionCommit();
    }
    else if (dwReason == DLL_PROCESS_DETACH) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)RealProcessNext, OurProcess32Next);
        DetourDetach(&(PVOID&)TrueSleep, TimedSleep);
        DetourTransactionCommit();
    }
    return TRUE;
}