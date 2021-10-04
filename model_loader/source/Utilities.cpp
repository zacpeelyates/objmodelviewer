
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: Utilities.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 30/09/21
// Last Edited:  04/10/21
// Brief: Collection of useful function implementations used across the project that don't have a home anywhere else :( 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Utilities.h"
#include <fstream>
#include <sstream>
std::vector<std::string> SplitStringAtChar(std::string a_strData, char a_cDelimiter)
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

bool ParseStringToInt(std::string& a_rStrIn)
{
	//Checks if given string can be parsed to an integer
	if (a_rStrIn.empty()) return false;
	return std::find_if(a_rStrIn.begin(), a_rStrIn.end(), [](unsigned char c) {return !std::isdigit(c); }) == a_rStrIn.end();
}


GLuint CreateShader(const char* a_strShaderFile, unsigned int a_eShaderType)
{
	std::string strShaderCode;
	//open shader file
	std::ifstream shaderStream(a_strShaderFile);
	//if that worked ok, load file line by line
	if (shaderStream.is_open())
	{
		std::string Line = "";
		while (std::getline(shaderStream, Line))
		{
			strShaderCode += "\n" + Line;
		}
		shaderStream.close();
	}
	//convert to cstring
	char const* szShaderSourcePointer = strShaderCode.c_str();
	//create shader ID
	GLuint uiShader = glCreateShader(a_eShaderType);
	//load source code
	glShaderSource(uiShader, 1, &szShaderSourcePointer, NULL);
	//compile shader
	glCompileShader(uiShader);
	//check for compilation errors and output them
	GLint iStatus;
	glGetShaderiv(uiShader, GL_COMPILE_STATUS, &iStatus);
	if (iStatus == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(uiShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* strInfoLog = new GLchar[infoLogLength];
		glGetShaderInfoLog(uiShader, infoLogLength, NULL, strInfoLog);
		const char* strShaderType = NULL;
		switch (a_eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}
		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType,
			strInfoLog);
		delete[] strInfoLog;
	}
	return uiShader;
}

GLuint CreateProgram()
	{
		std::vector<GLuint> shaderList;
		shaderList.push_back(CreateShader("C:/Users/Zac/Documents/UoG/Level/5/Projects/SEM1/Low/ct5036-assessment-rendering-framework-ramkhisthrowing/model_loader/resource/shaders/vertex.glsl" //THIS IS BAD AND IDK WHY I CANT GET IT TO WORK ANOTHER WAY ,
			GL_VERTEX_SHADER));
		shaderList.push_back(CreateShader("C:/Users/Zac/Documents/UoG/Level/5/Projects/SEM1/Low/ct5036-assessment-rendering-framework-ramkhisthrowing/model_loader/resource/shaders/fragment.glsl",
			GL_FRAGMENT_SHADER));
		//create shader program ID
		GLuint uiProgram = glCreateProgram();
		//attach shaders
		for (auto shader = shaderList.begin(); shader != shaderList.end(); shader++)
			glAttachShader(uiProgram, *shader);
		//link program
		glLinkProgram(uiProgram);
		//check for link errors and output them
		GLint status;
		glGetProgramiv(uiProgram, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint infoLogLength;
			glGetProgramiv(uiProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
			GLchar* strInfoLog = new GLchar[infoLogLength];
			glGetProgramInfoLog(uiProgram, infoLogLength, NULL, strInfoLog);
			fprintf(stderr, "Linker failure: %s\n", strInfoLog);
			delete[] strInfoLog;
		}
		for (auto shader = shaderList.begin(); shader != shaderList.end(); shader++)
		{
			glDetachShader(uiProgram, *shader);
			glDeleteShader(*shader);
		}
		return uiProgram;
	}
