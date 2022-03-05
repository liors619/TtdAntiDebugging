
#include <iostream>
#include <windows.h>

using namespace std;

bool Check()
{
    __try
    {
        CloseHandle((HANDLE)0xDEADBEEF);
        return false;
    }
    __except (EXCEPTION_INVALID_HANDLE == GetExceptionCode()
        ? EXCEPTION_EXECUTE_HANDLER
        : EXCEPTION_CONTINUE_SEARCH)
    {
        return true;
    }
}

int main()
{
    system("pause");
    bool b = Check();
    if (b)
        cout << "Debugger Found\n";
    else
        cout << "Debugger not found\n";
    system("pause");
}
