#include "ShaderManager.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>


ShaderManager* ShaderManager::mInstance = nullptr;

ShaderManager* ShaderManager::CreateInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new ShaderManager();
	}
	else 
	{
		std::cout << "Tried to create more than one Shader Manager! (don't)" << std::endl;
	}
	 return mInstance;
}

ShaderManager* ShaderManager::GetInstance() 
{
	if (mInstance == nullptr)
	{
		return ShaderManager::CreateInstance();
	}
	else return mInstance;
}

void ShaderManager::DestroyInstance()
{
	if (mInstance != nullptr) 
	{
		delete mInstance;
		mInstance = nullptr;
	}
	else
	{
		std::cout << "Tried to delete null Shader Manager instance!" << std::endl;
	}
}

unsigned int ShaderManager::CreateProgram(const int& a_vertexShader, const int& a_fragmentShader) 
{
	ShaderManager* instance = ShaderManager::GetInstance();
	return instance->CreateProgramInternal(a_vertexShader, a_fragmentShader);
}

void ShaderManager::DeleteShader(unsigned int a_uiShaderID)
{
}

unsigned int ShaderManager::CreateProgramInternal(const int& a_vertexShader, const int& a_fragmentShader)
{
	int result = GL_FALSE;

	//create shader program and attach shaders 
	unsigned int handle = glCreateProgram();
	glAttachShader(handle, a_vertexShader);
	glAttachShader(handle, a_fragmentShader);
	//link shaders to program 
	glLinkProgram(handle);
	//check if program was created
	glGetProgramiv(handle, GL_LINK_STATUS, &result);
	if (result == GL_FALSE) 
	{
		int infoLogLength = 0;
		glGetProgramiv(handle,GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(handle, infoLogLength, 0, infoLog);
		std::cout << "Shader Linker Error" << std::endl;
		std::cout << infoLog << std::endl;

		delete[] infoLog;
		return 0; //null program return 
	}
	return result;

}

void ShaderManager::DeleteShaderInternal(unsigned int a_uiShaderID)
{
}

void ShaderManager::DeleteProgram(unsigned int a_program) 
{
	ShaderManager* instance = ShaderManager::GetInstance();
	instance->DeleteProgramInternal(a_program);
}

void ShaderManager::DeleteProgramInternal(unsigned int a_program) 
{
	for (std::map<std::string, unsigned int>::iterator iter = mPrograms.begin(); iter != mShaders.end(); ++iter)
	{
		glDeleteProgram(iter->second);
		mPrograms.erase(iter);
		break;
	}
}

ShaderManager::ShaderManager() 
{

}

ShaderManager::~ShaderManager() 
{
	//delete shaders
	for (std::map<std::string,unsigned int>::iterator iter = mShaders.begin(); iter != mShaders.end(); ++iter) 
	{
		glDeleteShader(iter->second);
	}
	//delete programs
	for (std::map<std::string, unsigned int>::iterator iter = mPrograms.begin(); iter != mPrograms.end(); ++iter)
	{
		glDeleteProgram(iter->second);
	}
}



GLuint ShaderManager::LoadShader(const char* a_strShaderFile, unsigned int a_eShaderType)
{
	ShaderManager* instance = ShaderManager::GetInstance();
	return instance->LoadShaderInternal(a_strShaderFile, a_eShaderType);

}

unsigned int ShaderManager::LoadShaderInternal(const char* a_filename,unsigned int a_type)
{
	//get shader from file
	char* source;
	GLuint shader = glCreateShader(a_type);
	//set source buffer & compile 
	glShaderSource(shader, 1, (const char**)&source, 0);
	glCompileShader(shader);
	//delete source as buffer has been allocated
	delete[] source;

	//test shader for errors
	int result = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &result);
		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(shader, infoLogLength, 0, infoLog);
		std::cout << "Compile Falied: " << a_filename << std::endl;
		std::cout << infoLog << std::endl;
		delete[] infoLog;
		return 0;
	}
	else 
	{
		mShaders.emplace(a_filename, shader);
	}
	return result;

}