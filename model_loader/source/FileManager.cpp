///////////////////////////////////////////////////////////////////////////////
// File: FileManager.cpp	
// Author: Zac Peel-Yates (s1703955)
// Date Created: 30/09/21
// Date Edited:  30/09/21
// Brief: Function implementation for handling file loading and file information.
// This information is NOT the parsing of file contents, its just information about the file like path and size
// For file parsing see the associated loader class (e.g OBJLoader.h)
///////////////////////////////////////////////////////////////////////////////
#include "FileManager.h"
#include "Utilities.h"
#include <iostream>

FileManager* FileManager::mInstance = nullptr;

FileManager::FileManager() 
{

}

FileManager::~FileManager() 
{
	
}

FileManager* FileManager::CreateInstance()
{
	if (mInstance == nullptr) 
	{
		mInstance = new FileManager();
	}
	else
	{
		std::cout << "Tried to create more than one File Manager!" << std::endl;
	}
	return mInstance;
}

FileManager* FileManager::GetInstance()
{
	if (mInstance == nullptr)
	{
		return FileManager::CreateInstance();
	}
	else return mInstance;
}

void FileManager::DestroyInstance()
{
	if(mInstance != nullptr)
	{
		delete mInstance;
		mInstance = nullptr;
	}
	else
	{
		std::cout << "Tried to delete null File Manager instance!" << std::endl;
	}
}

bool FileManager::LoadFile(std::string a_strFilePath)
{
	FileManager* instance = GetInstance();
	return instance->LoadFileInternal(a_strFilePath);
}

bool FileManager::LoadFileInternal(std::string a_strFilePath)
{
	
	FileInfo fileInfo;
	fileInfo.path = a_strFilePath;
	std::streamsize outSize = 0;
	Utilities::FileToBuffer(a_strFilePath, outSize);
	fileInfo.size = outSize;
	mFileMap.emplace(a_strFilePath, fileInfo);
	return true;
}

FileInfo FileManager::GetFileInfo(std::string a_strFilePath)
{
	FileManager* instance = GetInstance();
	return instance->GetFileInfoInternal(a_strFilePath);
}

FileInfo FileManager::GetFileInfoInternal(std::string a_strFilePath)
{
	return mFileMap.at(a_strFilePath);
}

