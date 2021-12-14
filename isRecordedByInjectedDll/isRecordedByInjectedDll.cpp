#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <psapi.h>
#include <string>

using namespace std;

int IsModuleLoaded(DWORD processID, string moduleName)
{
    HMODULE hMods[1024];
    HANDLE hProcess;
    DWORD cbNeeded;

    // Get a handle to the process.
    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
        PROCESS_VM_READ,
        FALSE, processID);
    if (NULL == hProcess)
        return -1;

    // Get a list of all the modules in this process.
    if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
    {
        for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
        {
            TCHAR szModName[MAX_PATH];

            // Get the base name of the module.
            if (GetModuleBaseName(hProcess, hMods[i], szModName,
                sizeof(szModName) / sizeof(TCHAR)))
            {
                char nameAsChars[1024];
                size_t nNumCharConverted;
                wcstombs_s(&nNumCharConverted, nameAsChars, 1024, szModName, 1024);
               
                if (strcmp(nameAsChars, moduleName.c_str()) == 0)
                    return 1;
            }
        }
    }

    // Release the handle to the process.
    CloseHandle(hProcess);
    return 0;
}

int main(void)
{
    int res = IsModuleLoaded(GetCurrentProcessId(), "TTDRecordCPU.dll");
    if (res == 1)
        cout << "ttd found!" << endl;
    else if (res == 0)
        cout << "ttd not found." << endl;
    else
        cout << "unexpected error." << endl;

    system("pause");
    return 0;
}