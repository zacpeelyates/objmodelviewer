#include "FileManager.h"
#include <iostream>

FileManager::FileManager(std::string a_strFilePath, std::fstream& a_oFileIn) : file(a_oFileIn)
{
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
	return path.substr(path.rfind('/') + 1, path.rfind('.'));
}

const std::string FileManager::GetType()
{
	return path.substr(path.rfind('.') + 1);
}

const std::string FileManager::GetDirectory()
{
	return path.substr(0, path.rfind('/') + 1);
}

long FileManager::GetProcessTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - time).count();
}

std::streamsize FileManager::GetFileSize()
{
	return bytes / 1024;
}

void FileManager::Print()
{
	std::cout << "\nName:\t" << this->GetName() << std::endl;
	std::cout << "Type:\t" << this->GetType() << std::endl;
	std::cout << "Size (File):\t" << this->GetFileSize() << "KB" << std::endl;
	std::cout << "Time to Process: " << this->GetProcessTime() << "ms" << std::endl;
}

