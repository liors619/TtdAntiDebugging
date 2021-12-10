
 // MAKE THE COMMENT SO IT WONT FAIL OUR BUILD WHEN WE ON 64BIT IN OTHER PROJECTS
#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <string>

using namespace std;
bool IsDebugged()
{
    __try
    {
        __asm
        {
            pushfd
            mov dword ptr[esp], 0x100
            popfd
            nop
        }
        return true;
    }
    __except (GetExceptionCode() == EXCEPTION_SINGLE_STEP
        ? EXCEPTION_EXECUTE_HANDLER
        : EXCEPTION_CONTINUE_EXECUTION)
    {
        return false;
    }
}

int main()
{
    bool b = IsDebugged();
    int value = 0;
    if (b) {
        cout << "Debugger Found\n";
        value = 10;
    }
    else {
        cout << "Debugger not found\n";
        value = 30;
    }
    system("pause");
}