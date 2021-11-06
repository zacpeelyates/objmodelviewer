
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

//camera function
void Utilities::FreeMovement(glm::mat4& a_m4Transform, float a_fDeltaTime, float a_fSpeed, const glm::vec3& a_v3Up)
{
	GLFWwindow* window = glfwGetCurrentContext();

	//get camera transform vectors
	glm::vec4 v4Right = a_m4Transform[0];
	glm::vec4 v4Up = a_m4Transform[1];
	glm::vec4 v4Forward = a_m4Transform[2];
	//get location vectors
	glm::vec4 v4Translation = a_m4Transform[3];
	//set speed variables (shift to sprint)
	float defactoSpeed = a_fDeltaTime * a_fSpeed;
	float framespeed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? defactoSpeed * 2.0f : defactoSpeed;


	if (glfwGetKey(window, 'W') == GLFW_PRESS)
	{
		v4Translation -= v4Forward * framespeed;
	}
	if (glfwGetKey(window, 'S') == GLFW_PRESS)
	{
		v4Translation += v4Forward * framespeed;
	}
	if (glfwGetKey(window, 'A') == GLFW_PRESS)
	{
		v4Translation -= v4Right * framespeed;
	}
	if (glfwGetKey(window, 'D') == GLFW_PRESS)
	{
		v4Translation += v4Right * framespeed;
	}
	if (glfwGetKey(window, 'Q') == GLFW_PRESS)
	{
		v4Translation += v4Up * framespeed;
	}
	if (glfwGetKey(window, 'E') == GLFW_PRESS)
	{
		v4Translation -= v4Up * framespeed;
	}

	a_m4Transform[3] = v4Translation;

	//rotation
	static bool sbMouse2ButtonDown = false;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		static double sdPrevMouseX = 0;
		static double sdPrevMouseY = 0;
		if (sbMouse2ButtonDown == false)
		{
			sbMouse2ButtonDown = true;
			glfwGetCursorPos(window, &sdPrevMouseX, &sdPrevMouseY);
		}

		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		double dDeltaX = mouseX - sdPrevMouseX;
		double dDeltaY = mouseY - sdPrevMouseY;

		sdPrevMouseX = mouseX;
		sdPrevMouseY = mouseY;


		glm::mat4 m4Temp;
		if (dDeltaY != 0)
		{
			//set temp to pitch
			m4Temp = glm::axisAngleMatrix(v4Right.xyz(), (float)-dDeltaY / 150.0f);
			v4Right = m4Temp * v4Right;
			v4Up = m4Temp * v4Up;
			v4Forward = m4Temp * v4Forward;
		}

		if (dDeltaX != 0)
		{
			//set temp to yaw
			m4Temp = glm::axisAngleMatrix(a_v3Up, (float)-dDeltaX / 150.0f);
			v4Right = m4Temp * v4Right;
			v4Up = m4Temp * v4Up;
			v4Forward = m4Temp * v4Forward;
		}

		a_m4Transform[0] = v4Right;
		a_m4Transform[1] = v4Up;
		a_m4Transform[2] = v4Forward;
	}
	else
	{
		sbMouse2ButtonDown = false;
	}
}