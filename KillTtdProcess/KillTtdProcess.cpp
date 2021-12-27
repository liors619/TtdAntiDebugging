
// IsTtdProcessRunning.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <string>

using namespace std;

int GetPid(string executableName) {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (!Process32First(snapshot, &entry)) {
        CloseHandle(snapshot);
        return false;
    }

    do {
        TCHAR* currentProcessName = entry.szExeFile;
        //_tprintf(TEXT("%s\n"), currentProcessName);
        bool same = true;
        for (int i = 0; i < 7; i++) {
            if (executableName[i] != currentProcessName[i]) {
                same = false;
            }
        }
        if (same) {
            CloseHandle(snapshot);
            return entry.th32ProcessID;
        }
    } while (Process32Next(snapshot, &entry));

    CloseHandle(snapshot);
    return false;
}

void KillProcess(string executableName) {
    int process_id = GetPid(executableName);
    const auto process_to_kill = OpenProcess(PROCESS_TERMINATE, false, process_id);
    TerminateProcess(process_to_kill, 1);
    CloseHandle(process_to_kill);


}

int main()
{
    KillProcess("TTD.exe");
    //KillProcess("DbgX.Shell.exe");
    system("pause");
}

