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
    using namespace std;
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
            PROCESS_BASIC_INFORMATION pbi;

            DWORD dwProcessDebugFlags, dwReturned;
            NTSTATUS status = pfnNtQueryInformationProcess(
                GetCurrentProcess(),
                ProcessBasicInformation,
                &pbi,
                sizeof(PROCESS_BASIC_INFORMATION),
                &dwReturned);

            if (NT_SUCCESS(status))
            {
                PPEB peb_addr = pbi.PebBaseAddress;
                PDWORD something = (PDWORD)(((PBYTE)peb_addr) + 0xBC);
                DWORD value = *something;
                cout << "NtGlobalFlag value is: " << value << endl;
                if (value == 0)
                    return 0;
                else if (value == NT_GLOBAL_FLAG_DEBUGGED)
                    return 1;
                else
                    return -1;
            }
            else {
                return -1;
            }
            system("pause");
        }
    }
}


