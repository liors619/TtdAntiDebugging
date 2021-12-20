// DllInjectorByDigitalWhisper.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <string.h>
#include <string>
#include <TlHelp32.h>
#include <comdef.h>  // you will need this

#define WIN32_LEAN_AND_MEAN
#define DLL_QUERY_HMODULE 6
#define DEREF( name )*(UINT_PTR *)(name)
#define DEREF_64( name )*(DWORD64 *)(name)
#define DEREF_32( name )*(DWORD *)(name)
#define DEREF_16( name )*(WORD *)(name)
#define DEREF_8( name )*(BYTE *)(name)
typedef BOOL(WINAPI* DLLMAIN)(HINSTANCE, DWORD, LPVOID);
#define DLLEXPORT __declspec( dllexport ) 

using namespace std;

// Your wchar_t*
string convFunc(WCHAR* input) {
	wstring ws(input);
	// your new String
	string str(ws.begin(), ws.end());
	// Show String
	return str;
}

DWORD GetProcIdByName(LPCSTR name);
HANDLE WINAPI LoadRemoteLibraryR(HANDLE hProcess, LPVOID lpBuffer, DWORD dwLength, LPVOID lpParameter);
DWORD GetReflectiveLoaderOffset(VOID* lpReflectiveDllBuffer);
DWORD Rva2Offset(DWORD dwRva, UINT_PTR uiBaseAddress);

int main(int argc, char** argv)
{
	HANDLE hFile = NULL;
	HANDLE hTargetProcess = NULL;
	HANDLE hModule = NULL;
	DWORD dwLength = NULL;
	DWORD dwBytesRead = 0;
	DWORD dwTargetProcessID = NULL;
	LPVOID lpBuffer = NULL;
	string dllFullPath = "D:\\Users\\Lior\\Downloads\\D_downloads\\TtdSolution\\x64\\Debug\\Dll8.dll";
	string targetProcessName = "IsTtdProcessRunning.exe";

	dwTargetProcessID = GetProcIdByName(targetProcessName.c_str());
	if (!dwTargetProcessID) {
		ERROR("Could not find target process, maybe it is not running");
		return 0;
	}
	hFile = CreateFileA(dllFullPath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		cout << "Failed to open the DLL file" << endl;
		return 0;
	}
	dwLength = GetFileSize(hFile, NULL);
	if (dwLength == INVALID_FILE_SIZE || dwLength == 0)
	{
		cout << "Failed to get the DLL file size" << endl;
		return 0;
	}

	lpBuffer = HeapAlloc(GetProcessHeap(), 0, dwLength);
	if (!lpBuffer) {
		cout << "Failed to alloc a buffer!" << endl;
		return 0;
	}
		
	if (ReadFile(hFile, lpBuffer, dwLength, &dwBytesRead, NULL) == FALSE) {
		cout << "Failed to read dll raw data" << endl;
		return 0;
	}

	hTargetProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
		PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, dwTargetProcessID);
	if (!hTargetProcess)
	{
		cout << "Failed to open the target process" << endl;
		return 0;
	}

	hModule = LoadRemoteLibraryR(hTargetProcess, lpBuffer, dwLength, NULL);
	if (!hModule) {
		cout << "Failed to inject the DLL" << endl;
		return 0;
	}

	printf("[+] Injected the '%s' DLL into process %d.", dllFullPath, dwTargetProcessID);
	WaitForSingleObject(hModule, -1);
	return 1;
}

DWORD GetProcIdByName(LPCSTR name) {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD | TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 processEntry = { sizeof(PROCESSENTRY32) };
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hSnapshot);
		cout << "Could not create snapshot" << endl;
		return 0;
	}

	string targetProcessName = string("IsTtdProcessRunning.exe");
	if (Process32First(hSnapshot, &processEntry))
	{
		do
		{
			if (!Process32Next(hSnapshot, &processEntry)) 
			{
				cout << "Could not get next process" << endl;
				return 0;
			}

		} while (convFunc(processEntry.szExeFile).compare(targetProcessName) != 0);
	}
	else
	{
		cout << "Could not get first process in snapshot" << endl;
		return 0;
	}
	return processEntry.th32ProcessID;
}

HANDLE WINAPI LoadRemoteLibraryR(HANDLE hProcess, LPVOID lpBuffer, DWORD dwLength, LPVOID lpParameter)
{
	BOOL bSuccess = FALSE;
	LPVOID lpRemoteLibraryBuffer = NULL;
	LPTHREAD_START_ROUTINE lpReflectiveLoader = NULL;
	HANDLE hThread = NULL;
	DWORD dwReflectiveLoaderOffset = 0;
	DWORD dwThreadId = 0;
	dwReflectiveLoaderOffset = GetReflectiveLoaderOffset(lpBuffer);
	if (!dwReflectiveLoaderOffset)
		return hThread;
	lpRemoteLibraryBuffer = VirtualAllocEx(hProcess, NULL, dwLength, MEM_RESERVE | MEM_COMMIT,
		PAGE_EXECUTE_READWRITE);
	if (!lpRemoteLibraryBuffer)
		return hThread;
	if (!WriteProcessMemory(hProcess, lpRemoteLibraryBuffer, lpBuffer, dwLength, NULL))
		return hThread;

	lpReflectiveLoader = (LPTHREAD_START_ROUTINE)((ULONG_PTR)lpRemoteLibraryBuffer +
		dwReflectiveLoaderOffset);

	hThread = CreateRemoteThread(hProcess, NULL, 1024 * 1024, lpReflectiveLoader, lpParameter,
		(DWORD)NULL, &dwThreadId);
	return hThread;
}


DWORD GetReflectiveLoaderOffset(VOID* lpReflectiveDllBuffer)
{
	UINT_PTR uiBaseAddress = 0;
	UINT_PTR uiExportDir = 0;
	UINT_PTR uiNameArray = 0;
	UINT_PTR uiAddressArray = 0;
	UINT_PTR uiNameOrdinals = 0;
	DWORD dwCounter = 0;
	uiBaseAddress = (UINT_PTR)lpReflectiveDllBuffer;
	uiExportDir = uiBaseAddress + ((PIMAGE_DOS_HEADER)uiBaseAddress)->e_lfanew;
	uiNameArray = (UINT_PTR) & ((PIMAGE_NT_HEADERS)uiExportDir) -> OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
	uiExportDir = uiBaseAddress + Rva2Offset(((PIMAGE_DATA_DIRECTORY)uiNameArray)->VirtualAddress,
		uiBaseAddress);
	uiNameArray = uiBaseAddress + Rva2Offset(((PIMAGE_EXPORT_DIRECTORY)uiExportDir)->AddressOfNames,
		uiBaseAddress);
	uiAddressArray = uiBaseAddress + Rva2Offset(((PIMAGE_EXPORT_DIRECTORY)uiExportDir) -> AddressOfFunctions, uiBaseAddress);
	uiNameOrdinals = uiBaseAddress + Rva2Offset(((PIMAGE_EXPORT_DIRECTORY)uiExportDir) -> AddressOfNameOrdinals, uiBaseAddress);
	dwCounter = ((PIMAGE_EXPORT_DIRECTORY)uiExportDir)->NumberOfNames;
	while (dwCounter--)
	{
		char* cpExportedFunctionName = (char*)(uiBaseAddress + Rva2Offset(DEREF_32(uiNameArray),
			uiBaseAddress));
		if (strstr(cpExportedFunctionName, "ReflectiveLoader") != NULL)
		{
			uiAddressArray = uiBaseAddress + Rva2Offset(((PIMAGE_EXPORT_DIRECTORY)uiExportDir) -> AddressOfFunctions, uiBaseAddress);
			uiAddressArray += (DEREF_16(uiNameOrdinals) * sizeof(DWORD));
			return Rva2Offset(DEREF_32(uiAddressArray), uiBaseAddress);
		}

		uiNameArray += sizeof(DWORD);
		uiNameOrdinals += sizeof(WORD);
	}
	return 0;
}

DWORD Rva2Offset(DWORD dwRva, UINT_PTR uiBaseAddress)
{
	WORD wIndex = 0;
	PIMAGE_SECTION_HEADER pSectionHeader = NULL;
	PIMAGE_NT_HEADERS pNtHeaders = NULL;
	pNtHeaders = (PIMAGE_NT_HEADERS)(uiBaseAddress + ((PIMAGE_DOS_HEADER)uiBaseAddress)->e_lfanew);
	pSectionHeader = (PIMAGE_SECTION_HEADER)((UINT_PTR)(&pNtHeaders->OptionalHeader) + pNtHeaders -> FileHeader.SizeOfOptionalHeader);
	if (dwRva < pSectionHeader[0].PointerToRawData)
		return dwRva;
	for (wIndex = 0; wIndex < pNtHeaders->FileHeader.NumberOfSections; wIndex++)
	{
		if (dwRva >= pSectionHeader[wIndex].VirtualAddress && dwRva <
			(pSectionHeader[wIndex].VirtualAddress + pSectionHeader[wIndex].SizeOfRawData))
			return (dwRva - pSectionHeader[wIndex].VirtualAddress +
				pSectionHeader[wIndex].PointerToRawData);
	}
	return 0;
}
