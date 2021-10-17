
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: Utilities.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 30/09/21
// Last Edited:  04/10/21
// Brief: Collection of useful function implementations used across the project that don't have a home anywhere else :( 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Utilities.h"
#include <sstream>
#include <fstream>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

static double s_prevTime = 0;
static float  s_totalTime = 0;
static float  s_deltaTime = 0;

//time function definitions

void Utilities::TimerReset()
{
	s_prevTime = glfwGetTime();
	s_totalTime = 0;
	s_deltaTime = 0;
}

float Utilities::TimerTick()
{
	double currentTime = glfwGetTime();
	s_deltaTime = (float)(currentTime - s_prevTime);
	s_totalTime += s_deltaTime;
	s_prevTime = currentTime;
	return s_deltaTime;
}

float Utilities::getDeltaTime() 
{
	return s_deltaTime;
}

float Utilities::getTotalTime()
{
	return s_totalTime;
}

//file loading definitions 

char* Utilities::FileToBuffer(const std::string a_strFilePath)
{
	std::streamsize temp;
	return FileToBuffer(a_strFilePath, temp);
}


char* Utilities::FileToBuffer(const std::string a_strFilePath, std::streamsize& a_rFileSize)
{
	std::fstream file;
	file.open(a_strFilePath, std::ios_base::in | std::ios_base::binary);
	if (file.is_open())
	{
		file.ignore(std::numeric_limits<std::streamsize>::max());
		a_rFileSize = file.gcount();
		file.clear();
		file.seekg(0, std::ios_base::beg);
		if (a_rFileSize == 0)
		{
			file.close();
			return nullptr;
		}
		char* databuffer = new char[a_rFileSize + 1];
		memset(databuffer, 0, a_rFileSize + 1); //clear buffer
		file.read(databuffer, a_rFileSize);
		file.close();
		return databuffer;
	}
	return nullptr;
}

//file information utility definitions
std::string Utilities::GetFileName(const std::string a_strFilePath)
{
	//returns the file name with no preceeding path or extention
	return a_strFilePath.substr(a_strFilePath.rfind('/') + 1, a_strFilePath.rfind('.'));
}

std::string Utilities::GetFileType(const std::string a_strFilePath)
{
	//returns the file extention with no preceeding name or path
	return a_strFilePath.substr(a_strFilePath.rfind('.') + 1);
}

std::string Utilities::GetFileDirectory(const std::string a_strFilePath)
{
	//returns directory the file is contained in
	return a_strFilePath.substr(0, a_strFilePath.rfind('/') + 1);
}


//parsing function definitions
std::vector<std::string> Utilities::SplitStringAtChar(std::string a_strData, char a_cDelimiter)
{
	//Splits a string at give char and returns a vector of all the parts 
	std::vector<std::string> outVec;
	std::stringstream ss(a_strData);
	std::string segment;
	while (std::getline(ss, segment, a_cDelimiter))
	{
		outVec.push_back(segment);
	}
	return outVec;
}

bool Utilities::ParseStringToInt(std::string& a_rStrIn)
{
	//Checks if given string can be parsed to an integer
	if (a_rStrIn.empty()) return false;
	return std::find_if(a_rStrIn.begin(), a_rStrIn.end(), [](unsigned char c) {return !std::isdigit(c); }) == a_rStrIn.end();
}
