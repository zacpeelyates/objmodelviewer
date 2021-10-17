#include "ModelLoader.h"
#include "ShaderManager.h"
#include "Utilities.h"
#include "Camera.h"
#include "FileManager.h"
#include "OBJLoader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

ModelLoaderApp::ModelLoaderApp() {};
ModelLoaderApp::~ModelLoaderApp() {};

bool ModelLoaderApp::onCreate()
{
	//setup clear color, depth test, culling
	glClearColor(0.95f, 0.45f, 0.75f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearDepth(1.0f);
	//set shader program
	GLuint vertexShader = ShaderManager::LoadShader("resource/shaders/vertex.glsl", GL_VERTEX_SHADER);
	GLuint fragmentShader = ShaderManager::LoadShader("resource/shader/fragment.glsl", GL_FRAGMENT_SHADER);
	GLuint uiProgram = ShaderManager::CreateProgram(vertexShader, fragmentShader);
	//set viewport
	glViewport(0, 0, m_windowWidth, m_windowHeight);
	//create matricies
	m_cameraMatrix = glm::inverse(glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, (float)(m_windowWidth / m_windowHeight), 0.1f, 1000.0f);
	return true;
}



void ModelLoaderApp::Update(float deltaTime)
{
	FreeMovement(m_cameraMatrix,deltaTime);
}

void ModelLoaderApp::Draw() 
{
	//clear backbuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//enable shaders
	glUseProgram(m_uiProgram);
	//get view matrix from camera matrix in world space
	glm::mat4 viewMatrix = glm::inverse(m_cameraMatrix);
	glm::mat4 projectionViewMatrix = m_projectionMatrix * viewMatrix;
	//get projection view matrix var location from shader file
	int projectionViewMatrixUniformLocation = glGetUniformLocation(m_uiProgram, "ProjecionViewMatrix");
	//send pointer to location of matrix 
	glUniformMatrix4fv(projectionViewMatrixUniformLocation, 1, false, glm::value_ptr(projectionViewMatrix));
	//bind buffers 
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

 void ModelLoaderApp::Destroy() 
{
	//glDeleteBuffers(1, &location);
	ShaderManager::DeleteProgram(m_uiProgram);
}
