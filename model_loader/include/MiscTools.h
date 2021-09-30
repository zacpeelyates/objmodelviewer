////////////////////////////////////////////////////////
// File: MiscTools.h
// Author: Zac Peel-Yates (s1703955)
// Date Created: 30/09/21
// Last Edited:  30/09/21
// Brief: Function declarations for MiscTools.cpp 
///////////////////////////////////////////////////////
#ifndef __MISC_TOOLS_H__
#define __MISC_TOOLS_H__
#include <vector>
#include <string>
std::vector<std::string> SplitStringAtChar(std::string a_strData, char a_cDelimiter);
bool ParseStringToInt(std::string& a_rStrIn);
#endif // !__MISC_TOOLS_H__
