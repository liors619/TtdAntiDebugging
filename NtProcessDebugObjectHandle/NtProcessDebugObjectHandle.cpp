#include <windows.h>
#include <Winternl.h>
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>

#include <iostream>
#include <debugapi.h>

using namespace std;
int IsDebugged();

int main()
{
    auto is_debugged = IsDebugged();
    if (is_debugged == 1) {
        cout << "debugger found" << endl;
    }
    else if (is_debugged == 0) {
        cout << "debugger not found" << endl;
    }
    else {
        cout << "an error occurred" << endl;
    }
    system("pause");
}

// return 0 when no debugger found
// return 0 when debugger found
// return -1 when an error occurred
int IsDebugged() {
    typedef NTSTATUS(NTAPI* TNtQueryInformationProcess)(
        IN HANDLE           ProcessHandle,
        IN DWORD            ProcessInformationClass,
        OUT PVOID           ProcessInformation,
        IN ULONG            ProcessInformationLength,
        OUT PULONG          ReturnLength
        );

    HMODULE hNtdll = LoadLibraryA("ntdll.dll");
    if (hNtdll)
    {
        auto pfnNtQueryInformationProcess = (TNtQueryInformationProcess)GetProcAddress(
            hNtdll, "NtQueryInformationProcess");

        if (pfnNtQueryInformationProcess)
        {
            DWORD dwReturned;
            HANDLE hProcessDebugObject = 0;
            const DWORD ProcessDebugObjectHandle = 0x1e;
            NTSTATUS status = pfnNtQueryInformationProcess(
                GetCurrentProcess(),
                ProcessDebugObjectHandle,
                &hProcessDebugObject,
                sizeof(HANDLE),
                &dwReturned);

            if (NT_SUCCESS(status) && (0 != hProcessDebugObject))
                return 1;
            else return 0;
        }
    }
    return -1;
}


