#include <iostream>
#include <windows.h>
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
    BOOL is_debugged;
    if (CheckRemoteDebuggerPresent(GetCurrentProcess(), &is_debugged) == 0)
        return -1;
    else
        return is_debugged;
}
