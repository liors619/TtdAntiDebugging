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

int main(int, char* []) {
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE) {
		while (Process32Next(snapshot, &entry) == TRUE) {
			if (_stricmp(entry.szExeFile, "IsTtdProcessRunning.exe") == 0) {
				cout << "find process to inject the dll to" << endl;
				EnableDebugPriv();

				char dirPath[MAX_PATH];
				char fullPath[MAX_PATH];
				GetCurrentDirectory(MAX_PATH, dirPath);
				sprintf_s(fullPath, MAX_PATH, "%s\\Dll8.dll", dirPath);

				HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, entry.th32ProcessID);
				LPVOID libAddr = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
				LPVOID llParam = (LPVOID)VirtualAllocEx(hProcess, NULL, strlen(fullPath), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

				BOOL writeMemorySuccess = WriteProcessMemory(hProcess, llParam, fullPath, strlen(fullPath), NULL);
				HANDLE createThredSucceed = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)libAddr, llParam, NULL, NULL);
				system("pause");

				CloseHandle(hProcess);
				cout << "done injection" << endl;
			}
		}
	}
	system("pause");

	CloseHandle(snapshot);

	return 0;
}