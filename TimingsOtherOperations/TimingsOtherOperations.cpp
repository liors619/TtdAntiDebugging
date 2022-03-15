// TimingsOtherOperations.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include <fstream>
#include <windows.h>
#include <thread>

using namespace std;

void RunExceptionThrowing() {
    for (size_t i = 0; i < 10000; i++)
    {
        try {
            throw exception("message");
        }
        catch (exception &e) { }
    }
}

void NewAndDelete() {
    for (size_t i = 1; i < 10000; i++)
    {
        int* intP = new int;
        delete intP;
    }
}

void OpenAndCloseFiles() {
    for (size_t i = 0; i < 10000; i++)
    {
        ofstream myfile;
        myfile.open("example.txt", 2);
        myfile.close();
    }
}

void LoadAndUnloadLibraries() {
    for (size_t i = 0; i < 10000; i++)
    {
        HMODULE hNtdll = LoadLibraryA("ntdll.dll");
        int unloadResult = FreeLibrary(hNtdll);
        if (unloadResult == 0)
            printf("an error occured\n");
    }
}

void methodForNewThread() {
    int* intP = new int;
    delete intP;
}

void CreateAndRunThreadsWithNews() {
    for (size_t i = 0; i < 10000; i++)
    {
        std::thread t1(methodForNewThread);
        t1.join();
    }
}

int main()
{
    system("pause");
    while (true) {
        std::chrono::duration<double> elapsed_seconds;
        std::chrono::steady_clock::time_point startTime;
        std::chrono::steady_clock::time_point endTime;

        system("pause");
        startTime = std::chrono::steady_clock::now();
        RunExceptionThrowing();
        endTime = std::chrono::steady_clock::now();
        elapsed_seconds = endTime - startTime;
        cout << "exception time in seconds: " << elapsed_seconds.count() << endl;

        system("pause");
        startTime = std::chrono::steady_clock::now();
        NewAndDelete();
        endTime = std::chrono::steady_clock::now();
        elapsed_seconds = endTime - startTime;
        cout << "create and delete objects time in seconds: " << elapsed_seconds.count() << endl;
        
        system("pause");
        startTime = std::chrono::steady_clock::now();
        OpenAndCloseFiles();
        endTime = std::chrono::steady_clock::now();
        elapsed_seconds = endTime - startTime;
        auto elapsed_ticks = endTime - startTime;
        cout << "open and close files time in seconds: " << elapsed_seconds.count() << endl;

        system("pause");
        startTime = std::chrono::steady_clock::now();
        LoadAndUnloadLibraries();
        endTime = std::chrono::steady_clock::now();
        elapsed_seconds = endTime - startTime;
        cout << "load and unload libraries time in seconds: " << elapsed_seconds.count() << endl;

        system("pause");
        startTime = std::chrono::steady_clock::now();
        CreateAndRunThreadsWithNews();
        endTime = std::chrono::steady_clock::now();
        elapsed_seconds = endTime - startTime;
        cout << "threads time in seconds: " << elapsed_seconds.count() << endl;

        system("pause");
        printf("\n\n");
    }
}


