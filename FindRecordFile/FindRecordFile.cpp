// FindRecordFile.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include <windows.h>
#include <Psapi.h>
#include <string>

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
using namespace std;

string GetCurrentExecutableNameWithoutExtension() {
	char processExeName[MAX_PATH];
	GetModuleBaseNameA(GetCurrentProcess(), 0, processExeName, MAX_PATH);
	string processExeNameAsString(processExeName);
	return processExeNameAsString.substr(0, processExeNameAsString.find_last_of('.'));
}

int main()
{
	string exeNameWithoutExtension = GetCurrentExecutableNameWithoutExtension();

	for (const auto& dirEntry : recursive_directory_iterator("D:\\", filesystem::directory_options::skip_permission_denied)) {
		try {
			// we can check what our process name so researcher could not change it
			if (dirEntry.path().filename().string()._Starts_with(exeNameWithoutExtension) &&
				dirEntry.path().filename().extension().string() == ".run") {
				cout << dirEntry.path() << endl;
				system("pause");
			}
		}
		catch(exception &ex) { }
	}
}