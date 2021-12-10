#include <Windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <string>
#include <iostream>


using namespace std;

DWORD getParentPID(DWORD pid)
{
    HANDLE h = NULL;
    PROCESSENTRY32 pe = { 0 };
    DWORD ppid = 0;
    pe.dwSize = sizeof(PROCESSENTRY32);
    h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Process32First(h, &pe))
    {
        do
        {
            if (pe.th32ProcessID == pid)
            {
                ppid = pe.th32ParentProcessID;
                break;
            }
        } while (Process32Next(h, &pe));
    }
    CloseHandle(h);
    return (ppid);
}

int main(int argc, char* argv[])
{

    DWORD pid, parentProcessPid;
    int e;
    char fname[MAX_PATH] = { 0 };
    pid = GetCurrentProcessId();
    parentProcessPid = getParentPID(pid);
    
    //e = getProcessName(GetCurrentProcessId(), (LPWSTR)fname, MAX_PATH);
    
    HANDLE Handle = OpenProcess(
        PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
        FALSE,
        parentProcessPid /* This is the PID, you can find one from windows task manager */
    );
    if (Handle)
    {
        char Buffer[MAX_PATH];
        if (GetModuleBaseNameA(Handle, 0, Buffer, MAX_PATH))
        {
            // At this point, buffer contains the full path to the executable
            //printf("PPID=%d Err=%d EXE={%s}\n", ppid, 1, Buffer);
            //wstring ws(Buffer);
            // your new String
            //string str(ws.begin(), ws.end());
            // Show String
            cout << string(Buffer) << endl;
        }
        else
        {
            cout << "shouldnt be here" << endl;
            cout << GetLastError() << endl;
            // You better call GetLastError() here
        }
        CloseHandle(Handle);
    }
    system("pause");
}