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
//
//static VOID(WINAPI* TrueSleep)(DWORD dwMilliseconds) = Sleep;

BOOL(WINAPI* RealProcessNext)(HANDLE hSnapshot, LPPROCESSENTRY32 lppe) = Process32Next;

using namespace std;
BOOL OurProcess32Next(HANDLE hSnapshot, LPPROCESSENTRY32 lppe) {
    cout << "yo" << endl;
    return TRUE;
}


BOOL WINAPI DllMain(HINSTANCE, DWORD dwReason, LPVOID) {
    cout << dwReason << endl;
    switch (dwReason) {
    case DLL_PROCESS_ATTACH:
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)RealProcessNext, OurProcess32Next);
        DetourTransactionCommit();
        break;
        
    case DLL_PROCESS_DETACH:
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)RealProcessNext, OurProcess32Next);
        DetourTransactionCommit();
        break; 
    }
    

    return TRUE;
}
