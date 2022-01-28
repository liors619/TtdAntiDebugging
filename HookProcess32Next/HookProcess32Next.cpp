#include <Windows.h>
#include <iostream>
#include <string>
#include <tlhelp32.h>
#include <tchar.h>
#include <cstdio>
#include <cstdlib>
#include <clocale>
#include <cstdlib>
#include <detours.h>
#include <comdef.h>  // you will need this

using namespace std;

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

PROCESS_INFORMATION CreateProcessInPauseMode(std::string exePath, WCHAR* argv[])
{
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// Start the child process. 
	if (!CreateProcessW(
		s2ws(exePath).c_str(),   // No module name (use command line)
		argv[1],        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		CREATE_SUSPENDED | CREATE_NEW_CONSOLE,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
	}

	return pi;
}

void EnableDebugPriv() {
	HANDLE hToken;
	LUID luid;
	TOKEN_PRIVILEGES tkp;

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);

	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = luid;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(hToken, false, &tkp, sizeof(tkp), NULL, NULL);

	CloseHandle(hToken);
}

const char* ConvertToConstCharP(WCHAR* wc){
	_bstr_t b(wc);
	const char* c = b;
	return c;
}

LPWSTR ConvertToConstCharP(char* text) {
	wchar_t wtext[20];
	mbstowcs(wtext, text, strlen(text) + 1);//Plus null
	LPWSTR ptr = wtext;
	return ptr;
}

wchar_t* convertCharArrayToLPCWSTR(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}

int main(int, WCHAR* argv[]) {
	PROCESS_INFORMATION pi = CreateProcessInPauseMode("D:\\Users\\Lior\\Downloads\\D_downloads\\TtdSolution\\x64\\Debug\\FindWindow.exe", argv);

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE) {
		while (Process32Next(snapshot, &entry) == TRUE) {
			if (_stricmp(entry.szExeFile, "FindWindow.exe") == 0 || _stricmp(entry.szExeFile, "IsTtdProcessRunning.exe") == 0) {
				cout << "find process to inject the dll to: " << entry.szExeFile <<  endl;
				EnableDebugPriv();

				char dirPath[MAX_PATH];
				char fullPath[MAX_PATH];
				GetCurrentDirectory(MAX_PATH, dirPath);
				sprintf_s(fullPath, MAX_PATH, "D:\\Users\\Lior\\Downloads\\D_downloads\\TtdSolution\\x64\\Debug\\Dll8.dll", dirPath);

				HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, entry.th32ProcessID);
				LPVOID libAddr = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
				LPVOID llParam = (LPVOID)VirtualAllocEx(hProcess, NULL, strlen(fullPath), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

				BOOL writeMemorySuccess = WriteProcessMemory(hProcess, llParam, fullPath, strlen(fullPath), NULL);
				HANDLE createThredSucceed = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)libAddr, llParam, NULL, NULL);

				CloseHandle(hProcess);
				cout << "done injection" << endl;
			}
		}
	}

	CloseHandle(snapshot);

	system("pause");
	ResumeThread(pi.hThread);

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return 0;
}