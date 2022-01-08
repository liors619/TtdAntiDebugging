
// IsTtdProcessRunning.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <string>
#include <sys/stat.h>
#include <string>
#include <fstream>

using namespace std;

#include <string>

std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

VOID startup(std::string exePath, TCHAR* argv[])
{
    // additional information
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // set the size of the structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    std::string str = "";
    LPSTR args = const_cast<char*>(str.c_str());


    // start the program up
    CreateProcess(
        s2ws(exePath).c_str(),   // the path
        argv[1],        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
    );
    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

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

bool IsFileExists(const std::string& name) {
    ifstream f(name.c_str());
    return f.good();
}

// we can remove only run to remove the recording
// we remove the out so the next time they try to record our program it'll get the number 01 
void DeleteRecordFile() {
    if (IsFileExists("D:\\Users\\Lior\\Downloads\\D_downloads\\ttdProject\\Records\\RemoveRecordFileWithAnotherProcess01.run")){
        if (std::remove("D:\\Users\\Lior\\Downloads\\D_downloads\\ttdProject\\Records\\RemoveRecordFileWithAnotherProcess01.run") == 0) {
            std::remove("D:\\Users\\Lior\\Downloads\\D_downloads\\ttdProject\\Records\\RemoveRecordFileWithAnotherProcess01.out");
            std::remove("D:\\Users\\Lior\\Downloads\\D_downloads\\ttdProject\\Records\\RemoveRecordFileWithAnotherProcess01.idx");
            printf("record file deleted.\n");
        }
        else {
            printf("remove method failed.\n");
        }
    }
    else {
        printf("record file not found.\n");
    }
}

int main(int argc, TCHAR* argv[])
{
    Sleep(1000);
    KillProcess("DbgX.Shell.exe"); //kill windbg process
    Sleep(1000);
    DeleteRecordFile();
    startup("D:\\Users\\Lior\\Downloads\\D_downloads\\TtdSolution\\x64\\Debug\\RemoveRecordFileWithAnotherProcess.exe", argv);
}

