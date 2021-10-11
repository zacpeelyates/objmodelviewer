////////////////////////////////////////////////////////
// File: OBJLoader.h
// Author: Zac Peel-Yates (s1703955)
// Date Created: 30/09/21
// Last Edited:  30/09/21
// Brief: Function declarations for OBJLoader.cpp
///////////////////////////////////////////////////////
#ifndef __OBJ_LOADER_H__
#define __OBJ_LOADER_H__
#include "FileManager.h"
#include "OBJDatatypes.h"
#include <iostream>
#include <sstream>
class OBJLoader
{
public:
	bool OBJLoad(const std::string a_strFilePath, const bool a_bPrintComments, OBJData& LoadedData);
	bool OBJLoadMaterials(const std::string& a_strFilePath, OBJData& a_roLoadedData, const bool a_bPrintComments);
	OBJVertex OBJGetFaceFromVertex(std::string a_strFaceData, OBJData& a_roLoadedData);
	glm::vec4 OBJGetVectorFromValue(const std::string a_strValue);
	bool OBJGetKeyValuePairFromLine(const std::string& a_rStrLine, std::string& a_rStrOutKey, std::string& a_rStrOutValue);
	
	
};
#endif // !__OBJ_LOADER_H__