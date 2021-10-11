//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: Utilities.h
// Author: Zac Peel-Yates (s1703955)
// Date Created: 30/09/21
// Last Edited:  04/10/21
// Brief: Function definitions for Utilities.cpp
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __UTILITIES_H__
#define __UTILITIES_H__
#include <vector>
#include <string>
#include <glm/glm.hpp>
class Utilities
{
public:
	//utilities for string parsing
	static std::vector<std::string> SplitStringAtChar(std::string a_strData, char a_cDelimiter);
	static bool ParseStringToInt(std::string& a_rStrIn);

	//utilities for timing
	static void  TimerReset();
	static float TimerTick();
	static float getDeltaTime();
	static float getTotalTime();

	//utilites for file loading
	static char* FileToBuffer(const std::string a_strFilePath);
	static char* FileToBuffer(const std::string a_strFilePath, std::streamsize& a_rFileSize);

	//utilities for file handling
	static std::string GetFileDirectory(const std::string a_strFilePath);
	static std::string GetFileName(const std::string a_strFilePath);
	static std::string GetFileType(const std::string a_strFilePath);
};
#endif // !__UTILITIES_H__