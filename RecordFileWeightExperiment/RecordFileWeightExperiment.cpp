// TimingsOtherOperations.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include <fstream>
#include <windows.h>
#include <thread>

using namespace std;

int SecondsForEachMethod = 5;


void RunExceptionThrowing() {
    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds;

    elapsed_seconds = std::chrono::steady_clock::now() - startTime;
    while (elapsed_seconds.count() < SecondsForEachMethod) {
        try {
            throw exception("message");
        }
        catch (exception& e) {}
        elapsed_seconds = std::chrono::steady_clock::now() - startTime;
    }
}

void NewAndDelete() {
    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds;

    elapsed_seconds = std::chrono::steady_clock::now() - startTime;
    while (elapsed_seconds.count() < SecondsForEachMethod) {
        int* intP = new int;
        delete intP;
        elapsed_seconds = std::chrono::steady_clock::now() - startTime;
    }
}

void OpenAndCloseFiles() {
    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds;

    elapsed_seconds = std::chrono::steady_clock::now() - startTime;
    while (elapsed_seconds.count() < SecondsForEachMethod) {
        ofstream myfile;
        myfile.open("example.txt", 2);
        myfile.close();
        elapsed_seconds = std::chrono::steady_clock::now() - startTime;
    }
}

void LoadAndUnloadLibraries() {
    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds;

    elapsed_seconds = std::chrono::steady_clock::now() - startTime;
    while (elapsed_seconds.count() < SecondsForEachMethod) {
        HMODULE hNtdll = LoadLibraryA("ntdll.dll");
        int unloadResult = FreeLibrary(hNtdll);
        if (unloadResult == 0)
            printf("an error occured\n");
        elapsed_seconds = std::chrono::steady_clock::now() - startTime;
    }
}

void methodForNewThread() {
    int* intP = new int;
    delete intP;
}

void CreateAndRunThreads() {
    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds;

    elapsed_seconds = std::chrono::steady_clock::now() - startTime;
    while(elapsed_seconds.count() < SecondsForEachMethod)
    {
        std::thread t1(methodForNewThread);
        t1.join();
        elapsed_seconds = std::chrono::steady_clock::now() - startTime;
    }
}

int main()
{
    while (true) {
        std::chrono::duration<double> elapsed_seconds;
        std::chrono::steady_clock::time_point startTime;
        std::chrono::steady_clock::time_point endTime;

        startTime = std::chrono::steady_clock::now();
        CreateAndRunThreads();
        endTime = std::chrono::steady_clock::now();
        elapsed_seconds = endTime - startTime;
        cout << "threads time in seconds: " << elapsed_seconds.count() << endl;

        printf("\n\n");
    }
}


