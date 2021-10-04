//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: Utilities.h
// Author: Zac Peel-Yates (s1703955)
// Date Created: 30/09/21
// Last Edited:  04/10/21
// Brief: Function definitions for Utilities.cpp
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __UTILITIES_H__
#define __UTILITIES_H__
#include <glad/glad.h>
#include <vector>
#include <string>
	static GLuint CreateShader(const char* a_strShaderFile, unsigned int a_eShaderType);
    GLuint CreateProgram();
	std::vector<std::string> SplitStringAtChar(std::string a_strData, char a_cDelimiter);
	bool ParseStringToInt(std::string& a_rStrIn);
#endif // !__UTILITIES_H__