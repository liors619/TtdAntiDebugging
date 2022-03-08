
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
#include <filesystem>
#include <Psapi.h>

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
using namespace std;




string str2;

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

string GetCurrentExecutableNameWithoutExtension() {
    char processExeName[MAX_PATH];
    GetModuleBaseNameA(GetCurrentProcess(), 0, processExeName, MAX_PATH);
    string processExeNameAsString(processExeName);
    return processExeNameAsString.substr(0, processExeNameAsString.find_last_of('.'));
}

void DeleteRecordFiles(string recordedProgramName) {
    string exeNameWithoutExtension = GetCurrentExecutableNameWithoutExtension();

    for (const auto& dirEntry : recursive_directory_iterator("D:\\", filesystem::directory_options::skip_permission_denied)) {
        try {
            // we can check what our process name so researcher could not change it
            if (dirEntry.path().filename().string()._Starts_with(recordedProgramName) &&
                dirEntry.path().filename().extension().string() == ".run") {
                std::remove(dirEntry.path().string().c_str());
            }
        }
        catch (exception& ex) {}
    }
}

int main(int argc, char* argv[])
{
    std::string recordedProgramName(argv[1]);

    Sleep(1000);
    KillProcess("DbgX.Shell.exe"); //kill windbg process
    Sleep(1000);
    DeleteRecordFiles(recordedProgramName);
}

