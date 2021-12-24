// TimingsOtherOperations.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include <fstream>


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

void PrintMessages() {
    for (size_t i = 0; i < 1000; i++)
    {
        cout << "print something" << endl;
    }
}

void NewAndDelete() {
    for (size_t i = 0; i < 100000; i++)
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

}

void CreateAndRunThreads() {

}

int main()
{
    std::chrono::duration<double> elapsed_seconds;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point endTime;
    
    //system("pause");

    startTime = std::chrono::steady_clock::now();
    RunExceptionThrowing();
    endTime = std::chrono::steady_clock::now();
    elapsed_seconds = endTime - startTime;
    cout << "exception time in seconds: " << elapsed_seconds.count() << endl;

    //system("pause");

    //startTime = std::chrono::steady_clock::now();
    //PrintMessages();
    //endTime = std::chrono::steady_clock::now();
    //elapsed_seconds = endTime - startTime;
    //cout << "printing messages time in seconds: " << elapsed_seconds.count() << endl;

    //system("pause");

    startTime = std::chrono::steady_clock::now();
    NewAndDelete();
    endTime = std::chrono::steady_clock::now();
    elapsed_seconds = endTime - startTime;
    cout << "create and delete objects time in seconds: " << elapsed_seconds.count() << endl;

    //system("pause");

    startTime = std::chrono::steady_clock::now();
    OpenAndCloseFiles();
    endTime = std::chrono::steady_clock::now();
    elapsed_seconds = endTime - startTime;
    auto elapsed_ticks = endTime - startTime;
    //cout << "open and close files time in ticks: " << elapsed_ticks.count() << endl;
    cout << "open and close files time in seconds: " << elapsed_seconds.count() << endl;

    //system("pause");

    startTime = std::chrono::steady_clock::now();
    LoadAndUnloadLibraries();
    endTime = std::chrono::steady_clock::now();
    elapsed_seconds = endTime - startTime;
    //cout << "load and unload libraries time in seconds: " << elapsed_seconds.count() << endl;

    startTime = std::chrono::steady_clock::now();
    CreateAndRunThreads();
    endTime = std::chrono::steady_clock::now();
    elapsed_seconds = endTime - startTime;
    //cout << "threads time in ticks: " << elapsed_seconds.count() << endl;


    system("pause");
}


