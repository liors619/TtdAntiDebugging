// FindWindow.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <windows.h>

using namespace std;

int main()
{
    if (NULL != FindWindowA(NULL, "Recording")) {
        cout << "recording found" << endl;
    }
    else {
        cout << "recording not found" << endl;
    }

    system("pause");
}
