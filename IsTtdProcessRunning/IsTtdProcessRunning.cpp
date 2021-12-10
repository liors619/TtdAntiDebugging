
// IsTtdProcessRunning.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <string>

using namespace std;

bool IsProcessRunning(string executableName) {
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
            return true;
        }
    } while (Process32Next(snapshot, &entry));

    CloseHandle(snapshot);
    return false;
}

int main()
{
    while (true) {
        bool b = IsProcessRunning("TTD.exe");
        if (b)
            cout << "TTD.exe found\n";
        else
            cout << "TTD.exe not found\n";
        system("pause");
    }

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

