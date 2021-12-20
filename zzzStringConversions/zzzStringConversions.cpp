// zzzStringConversions.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <string.h>
#include <chrono>
#include <iostream>

// const char* to LPCWSTR
wchar_t* convertCharArrayToLPCWSTR(const char* charArray)
{
    wchar_t* wString = new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
    return wString;
}

LPWSTR converConstCharPointerToLpwstr(const char* str)
{
    wchar_t wtext[4096];
    size_t  size = 4097;
    mbstowcs_s(&size, wtext, str, strlen(str) + 1);//Plus null
    LPWSTR ptr = wtext;
    return ptr;
}


int main()
{
    std::cout << "Hello World!\n";
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
