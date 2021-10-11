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
#include <vector>
#include <fstream>
#include <map>


class FileManager
{
public:

	static FileManager* CreateInstance();
	static FileManager* GetInstance();
	static void DestroyInstance();

	static bool LoadFile(std::string a_strFilePath);


private:
	FileManager();
	~FileManager();
	std::vector<std::string> mFilePaths;
    bool LoadFileInternal(std::string a_strFilePath);
    static FileManager* mInstance;

};
#endif // !__FILE_MANAGER_H__