// Heap Flags and ForceFlags.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>

#include <windows.h>
#include <winternl.h>
#include <Winternl.h>
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#define FLG_HEAP_ENABLE_TAIL_CHECK   0x10
#define FLG_HEAP_ENABLE_FREE_CHECK   0x20
#define FLG_HEAP_VALIDATE_PARAMETERS 0x40
#define NT_GLOBAL_FLAG_DEBUGGED (FLG_HEAP_ENABLE_TAIL_CHECK | FLG_HEAP_ENABLE_FREE_CHECK | FLG_HEAP_VALIDATE_PARAMETERS)


#include <iostream>
#include <windows.h>
#include <debugapi.h>

using namespace std;
bool check() {
    PPEB pPeb = (PPEB)__readgsqword(0x60);
    PVOID pHeapBase = (PVOID)(*(PDWORD_PTR)((PBYTE)pPeb + 0x30));
    DWORD dwHeapFlagsOffset = true
        ? 0x70
        : 0x14;
    DWORD dwHeapForceFlagsOffset = true
        ? 0x74
        : 0x18;

    PDWORD pdwHeapFlags = (PDWORD)((PBYTE)pHeapBase + dwHeapFlagsOffset);
    PDWORD pdwHeapForceFlags = (PDWORD)((PBYTE)pHeapBase + dwHeapForceFlagsOffset);
    return (*pdwHeapFlags & ~HEAP_GROWABLE) || (*pdwHeapForceFlags != 0);
}


int main()
{
    auto is_debugged = check();
    if (is_debugged) {
        cout << "debugger found" << endl;
    }
    else {
        cout << "debugger not found" << endl;
    }
    system("pause");
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
