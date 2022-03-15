
#include <iostream>
#include <windows.h>

using namespace std;

bool Check()
{
    __try
    {
        CloseHandle((HANDLE)0xDEADC0DE);
        return false;
    }
    __except (EXCEPTION_INVALID_HANDLE == GetExceptionCode())
    {
        return true;
    }
}

int main()
{
    bool b = Check();
    if (b)
        cout << "Debugger Found\n";
    else
        cout << "Debugger not found\n";
}
