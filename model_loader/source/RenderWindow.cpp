#include "RenderWindow.h"
#include "ShaderManager.h"
#include "Utilities.h"
#include "obj_Loader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

RenderWindow::RenderWindow() {};
RenderWindow::~RenderWindow() {};

bool RenderWindow::onCreate()
{
	//setup clear color, depth test, culling
	glClearColor(0.95f, 0.45f, 0.75f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearDepth(1.0f);
	//set viewport
	glViewport(0, 0, m_windowWidth, m_windowHeight);
	//create matricies
	m_cameraMatrix = glm::inverse(glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, (float)(m_windowWidth / m_windowHeight), 0.1f, 1000.0f);
	
	// get filepath from user
	std::string path;
	bool comments = true; //remove this
	if ((m_objModel = OBJLoader::OBJProcess(path,comments)) != nullptr)
	{
		//set shader program
		GLuint vertexShader = ShaderManager::LoadShader("resource/shaders/obj_vertex.glsl", GL_VERTEX_SHADER);
		GLuint fragmentShader = ShaderManager::LoadShader("resource/shaders/obj_fragment.glsl", GL_FRAGMENT_SHADER);
		m_objProgram = ShaderManager::CreateProgram(vertexShader, fragmentShader);
		glGenBuffers(2, m_objModelBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_objModelBuffer[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return true;
	}
	else
	{
		return false;
	}

}


 

void RenderWindow::Update(float deltaTime)
{
	Utilities::FreeMovement(m_cameraMatrix,deltaTime);
}

void RenderWindow::Draw() 
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
	

	//unbind buffer/arrays and release program
	glDisableVertexAttribArray(0); //position
	glDisableVertexAttribArray(1); //normal
	glDisableVertexAttribArray(2); //uvcoord
	glUseProgram(0);
}

 void RenderWindow::Destroy() 
{
	//glDeleteBuffers(1, &location);
	ShaderManager::DeleteProgram(m_uiProgram);
}
