// TimingIsRecordedImplementation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <chrono>

using namespace std;

void LoadAndUnloadLibraries() {
    for (size_t i = 0; i < 50000; i++)
    {
        HMODULE hNtdll = LoadLibraryA("ntdll.dll");
        int unloadResult = FreeLibrary(hNtdll);
        if (unloadResult == 0)
            printf("an error occured\n");
    }
}

// 1 recorded
// 0 not recorded
// other - error occurred
int IsRecorded() {
    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
    LoadAndUnloadLibraries();
    std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = endTime - startTime;

    cout << "load and unload libraries time in seconds: " << elapsed_seconds.count() << endl;
    if (elapsed_seconds.count() >= 1)
        return 1;
    else
        return 0;

}

int main()
{
    auto is_debugged = IsRecorded();
    if (is_debugged == 1) {
        cout << "TTD found" << endl;
    }
    else if (is_debugged == 0) {
        cout << "TTD not found" << endl;
    }
    else {
        cout << "an error occurred" << endl;
    }
    system("pause");
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
