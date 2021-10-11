////////////////////////////////////////////////////////
// File: FileManager.h
// Author: Zac Peel-Yates (s1703955)
// Date Created: 30/09/21
// Last Edited:  30/09/21
// Brief: Function declarations for FileManager.cpp
///////////////////////////////////////////////////////
#ifndef __FILE_MANAGER_H__
#define __FILE_MANAGER_H__
#include "FileInfo.h"
#include <string>
#include <fstream>
#include <map>


class FileManager
{
public:

	static FileManager* CreateInstance();
	static FileManager* GetInstance();
	static void DestroyInstance();

	static bool LoadFile(std::string a_strFilePath);
	static FileInfo GetFileInfo(std::string a_strFilePath);


private:
	FileManager();
	~FileManager();
    bool LoadFileInternal(std::string a_strFilePath);
    FileInfo GetFileInfoInternal(std::string a_strFilePath);
    static  FileManager* mInstance;
	std::map<std::string, FileInfo> mFileMap;

};
#endif // !__FILE_MANAGER_H__