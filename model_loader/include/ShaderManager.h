
#ifndef __SHADER_UTILITIES_H__
#define __SHADER_UTILITIES_H__
#include <glad/glad.h>
#include <string>
#include <map>
class ShaderManager {

public:

	static unsigned int LoadShader(const char* ac_strShaderFile, unsigned int a_uiShaderType);
	static unsigned int CreateProgram(const int& a_irVertexShader, const int& a_irFragmentShader);
	static void DeleteShader(unsigned int a_uiShaderID);
	static void DeleteProgram(unsigned int a_uiProgramID);

	static ShaderManager* CreateInstance();
	static ShaderManager* GetInstance();
	static void DestroyInstance();

private:
	ShaderManager();
	~ShaderManager();

	static ShaderManager* mInstance;

	std::map<std::string, unsigned int> mShaders;
	std::map<std::string, unsigned int> mPrograms;

	unsigned int LoadShaderInternal(const char* a_strShaderFile, unsigned int a_eShaderType);
	unsigned int CreateProgramInternal(const int& a_irVertexShader, const int& a_irFragmentShader);
	void DeleteShaderInternal(unsigned int a_uiShaderID);
	void DeleteProgramInternal(unsigned int a_uiProgramID);
	
};
#endif // !__SHADER_UTILITIES_H__

