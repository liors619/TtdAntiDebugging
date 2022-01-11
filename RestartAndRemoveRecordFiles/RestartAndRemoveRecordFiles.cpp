// RemoveRecordFileWithAnotherProcess.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
#include <tlhelp32.h>

#include <Psapi.h>
#include <filesystem>

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
using namespace std;

wstring s2ws(const std::string& s)
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

LPWSTR converConstCharPointerToLpwstr(const char* str)
{
    wchar_t wtext[4096];
    size_t  size = 4097;
    mbstowcs_s(&size, wtext, str, strlen(str) + 1);//Plus null
    LPWSTR ptr = wtext;
    return ptr;
}

bool IsRecorded() {
    return FindWindowA(NULL, "Recording") != NULL;
}

bool IsFileExists(const std::string& name) {
    ifstream f(name.c_str());
    return f.good();
}

// we can remove only ".run" file to remove the recording
// we remove the ".out" file so the next time they try to record our program it'll get the number 01 
void DeleteFile(string fileName) {
    if (IsFileExists(fileName.c_str())) {
        if (std::remove(fileName.c_str()) == 0) {
            cout << "file deleted:    " << fileName << endl;
        }
        else {
            cout << "remove method failed." << endl;
        }
    }
    else {
        cout << "file not found.\n" << endl;
    }
}

bool IsNotFirstCall(int mainArgc) {
    return mainArgc > 1;
}

string GetCurrentExecutableNameWithoutExtension() {
    char processExeName[MAX_PATH];
    GetModuleBaseNameA(GetCurrentProcess(), 0, processExeName, MAX_PATH);
    string processExeNameAsString(processExeName);
    return processExeNameAsString.substr(0, processExeNameAsString.find_last_of('.'));
}

void FindAndDeleteRecordFiles()
{
    string exeNameWithoutExtension = GetCurrentExecutableNameWithoutExtension();

    for (const auto& dirEntry : recursive_directory_iterator("D:\\", std::filesystem::directory_options::skip_permission_denied)) {
        try {
            string fileName = dirEntry.path().filename().string();
            string extension = dirEntry.path().filename().extension().string();
            // we can check what our process name so researcher could not change it
            if (fileName._Starts_with(exeNameWithoutExtension) &&
                (extension == ".run" || extension == ".out" || extension == ".idx")) {
                DeleteFile(dirEntry.path().string());
            }
        }
        catch (exception& ex) {}
    }
}

void KillWinDbgAndDeleteRecordFiles()
{
    cout << "kill windbg process..." << endl;
    Sleep(1000);
    KillProcess("DbgX.Shell.exe"); //kill windbg process
    Sleep(1000);
    cout << "search for recordFiles..." << endl;
    FindAndDeleteRecordFiles();
}

void RunNewProcessOfOurself() {
    HWND dummyHWND = ::CreateWindowA("STATIC", "dummy", WS_VISIBLE, 0, 0, 100, 100, NULL, NULL, NULL, NULL);
    ShellExecute(dummyHWND, (s2ws("open")).c_str(), s2ws("D:\\Users\\Lior\\Downloads\\D_downloads\\TtdSolution\\x64\\Debug\\RestartAndRemoveRecordFiles.exe").c_str(), s2ws("secondRun").c_str(), NULL, SW_SHOWDEFAULT);
}

int main(int argc, TCHAR* argv[])
{
    if (IsNotFirstCall(argc))
        KillWinDbgAndDeleteRecordFiles();

    cout << "Hello World!" << endl;
    while (true) {
        Sleep(8000);
        if (IsRecorded()) {
            RunNewProcessOfOurself();
            return 0;
        }
    }
}