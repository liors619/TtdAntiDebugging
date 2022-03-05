
#include <iostream>
#include <windows.h>
#include <winternl.h>
#include <Winternl.h>
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <debugapi.h>

using namespace std;
bool check() {
    PPEB pPeb = (PPEB)__readgsqword(0x60);
    PVOID pHeapBase = (PVOID)(*(PDWORD_PTR)((PBYTE)pPeb + 0x30));
    DWORD dwHeapFlagsOffset = 0x70;
    DWORD dwHeapForceFlagsOffset = 0x74;

    PDWORD pdwHeapFlags = (PDWORD)((PBYTE)pHeapBase + dwHeapFlagsOffset);
    PDWORD pdwHeapForceFlags = (PDWORD)((PBYTE)pHeapBase + dwHeapForceFlagsOffset);
    return (*pdwHeapFlags & ~HEAP_GROWABLE) || (*pdwHeapForceFlags != 0);
}


int main()
{
    system("pause");
    auto is_debugged = check();
    if (is_debugged) {
        cout << "debugger found" << endl;
    }
    else {
        cout << "debugger not found" << endl;
    }
    system("pause");
}
