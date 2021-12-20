// TimingsOpenDialog.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


using namespace std;


// Returns an empty string if dialog is canceled
#include <windows.h>
#include <string.h>
#include <chrono>
#include <iostream>
#include <windows.h>
#include <shobjidl.h> 
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



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);

    /*
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog* pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);
            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Display the file name to the user.
                    if (SUCCEEDED(hr))
                    {
                        cout << "yo!" << endl;
                        MessageBoxW(NULL, pszFilePath, L"File Path", MB_OK);
                        cout << "yo!" << endl;
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
    */
    return 0;
}

int main()
{
    auto startTime = std::chrono::steady_clock::now();
    wWinMain(NULL, NULL, NULL, 0);
    auto endTime = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed_seconds = endTime - startTime;
    // the number of seconds we check, decided by observasions.. (when recorded the logic takes about 10x times)
    if (elapsed_seconds.count() > 0.01) {
        cout << "process recorded" << endl;
    }
    else {
        cout << "process not recorded" << endl;
    }

    cout << "seconds: " << elapsed_seconds.count() << endl;
    system("pause");
}


