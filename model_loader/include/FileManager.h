////////////////////////////////////////////////////////
// File: FileManager.h
// Author: Zac Peel-Yates (s1703955)
// Date Created: 30/09/21
// Last Edited:  30/09/21
// Brief: Function declarations for FileManager.cpp
///////////////////////////////////////////////////////
#ifndef __FILE_MANAGER_H__
#define __FILE_MANAGER_H__
#include <string>
#include <fstream>
#include <chrono>
class FileManager
{
public:
	FileManager(std::string a_strFilePath, std::fstream& a_oFileIn);
	const std::string GetName();
	const std::string GetType();
	const std::string GetDirectory();
	long GetProcessTime();
	std::streamsize GetFileSize();
	void Print();

	std::string path;
	std::fstream& file;
	std::streamsize bytes;
	bool initialized;
	std::chrono::time_point<std::chrono::steady_clock> time;

};
#endif // !__FILE_MANAGER_H__