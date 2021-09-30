///////////////////////////////////////////////////////////////////////////////
// File: FileManager.cpp	
// Author: Zac Peel-Yates (s1703955)
// Date Created: 30/09/21
// Date Edited:  30/09/21
// Brief: Function implementation for handling file loading and file information.
// This information is NOT the parising of file contents, its just information about the file like path and size
// For file parsing see the associated loader class (e.g OBJLoader.h)
///////////////////////////////////////////////////////////////////////////////
#include "FileManager.h"
#include <iostream>
FileManager::FileManager(std::string a_strFilePath, std::fstream& a_oFileIn) : file(a_oFileIn)
{
	//Constructor for FileManager class, sets associated variables and tries to load file at given path
	time = std::chrono::high_resolution_clock::now();
	path = a_strFilePath;
	initialized = false;
	std::cout << "Initializing file: " << path << std::endl;
	file.open(path, std::ios_base::in | std::ios_base::binary);
	if (file.is_open())
	{
		file.ignore(std::numeric_limits<std::streamsize>::max());
		bytes = file.gcount();
		if (bytes == 0)
		{
			std::cout << "File is empty. Closing." << std::endl;
			file.close();
		}
		else
		{
			file.seekg(0, std::ios_base::beg);
			initialized = true;
			std::cout << "File initialized" << std::endl;
		}
	}
	else
	{
		std::cout << "Could not open file." << std::endl;
	}
}

const std::string FileManager::GetName()
{
	//returns the file name with no preceeding path or extention
	return path.substr(path.rfind('/') + 1, path.rfind('.'));
}

const std::string FileManager::GetType()
{
	//returns the file extention with no preceeding name or path
	return path.substr(path.rfind('.') + 1);
}

const std::string FileManager::GetDirectory()
{
	//returns directory the file is contained in
	return path.substr(0, path.rfind('/') + 1);
}

long FileManager::GetProcessTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - time).count();
}

std::streamsize FileManager::GetFileSize()
{
	//returns file size in KB
	return bytes / 1024;
}

void FileManager::Print()
{
	std::cout << "\nName:\t" << this->GetName() << std::endl;
	std::cout << "Type:\t" << this->GetType() << std::endl;
	std::cout << "Size (File):\t" << this->GetFileSize() << "KB" << std::endl;
	std::cout << "Time to Process: " << this->GetProcessTime() << "ms" << std::endl;
}

