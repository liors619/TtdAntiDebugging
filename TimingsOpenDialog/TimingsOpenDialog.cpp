// TimingsOpenDialog.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


using namespace std;


// Returns an empty string if dialog is canceled
#include <windows.h>
#include <string.h>
#include <chrono>
#include <iostream>
using namespace std;


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



// Returns an empty string if dialog is canceled
string openfilename() {
    OPENFILENAME ofn;
    char fileName[MAX_PATH] = "";
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = convertCharArrayToLPCWSTR("All Files (*.*)\0*.*\0");
    ofn.lpstrFile = converConstCharPointerToLpwstr(fileName);
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = convertCharArrayToLPCWSTR("");

    string fileNameStr;

    if (GetOpenFileName(&ofn))
        fileNameStr = fileName;

    return fileNameStr;
}


int main()
{
    system("pause");
    auto startTime = std::chrono::steady_clock::now();
    openfilename();
    auto endTime = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed_seconds = endTime - startTime;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

    system("pause");
}


