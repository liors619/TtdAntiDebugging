// FindRecordFile.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
using namespace std;

int main()
{
	for (const auto& dirEntry : recursive_directory_iterator("D:\\", std::filesystem::directory_options::skip_permission_denied)) {
		try {
			// we can check what our process name so researcher could not change it
			if (dirEntry.path().filename().string()._Starts_with("RecordedProgramName") && 
				dirEntry.path().filename().extension().string() == ".run") {
				cout << dirEntry.path() << endl;
				system("pause");
			}
		}
		catch(exception &ex) { }
	}
}