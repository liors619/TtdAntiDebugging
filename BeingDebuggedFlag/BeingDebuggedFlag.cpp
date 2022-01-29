

#include <windows.h>
#include <Winternl.h>
#include <tchar.h>
#include <stdio.h>

#include <iostream>
#include <debugapi.h>

using namespace std;
int IsDebugged();

int main()
{
    system("pause");
    auto is_debugged = IsDebugged();
    if (is_debugged == 1) {
        cout << "debugger found" << endl;
    }
    else if (is_debugged == 0) {
        cout << "debugger not found" << endl;
    }
    else {
        cout << "an error occurred" << endl;
    }
    system("pause");
}

// return 0 when no debugger found
// return 0 when debugger found
// return -1 when an error occurred
int IsDebugged() {
    PPEB pPeb = (PPEB)__readgsqword(0x60);

    if (pPeb->BeingDebugged)
        return 1;
    else
        return 0;
}


