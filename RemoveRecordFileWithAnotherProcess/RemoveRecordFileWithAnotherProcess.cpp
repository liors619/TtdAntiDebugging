// RemoveRecordFileWithAnotherProcess.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>

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

bool IsRecorded() {
    return FindWindowA(NULL, "Recording") != NULL;
}


int main(int argc, TCHAR* argv[])
{
    TryDeleteRecordFiles();
    std::cout << "Hello World!\n";
    while (true) {
        Sleep(8000);
        if (IsRecorded()) {
            startup("D:\\Users\\Lior\\Downloads\\D_downloads\\TtdSolution\\x64\\Debug\\KillTtdProcessAndRemoveRecordFile.exe", argv);
            return 0;
        }
    }
}
