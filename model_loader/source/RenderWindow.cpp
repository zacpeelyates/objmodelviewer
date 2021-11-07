#include "RenderWindow.h"
#include "ShaderManager.h"
#include "Utilities.h"
#include "obj_Loader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>

RenderWindow::RenderWindow() {};
RenderWindow::~RenderWindow() {};

bool RenderWindow::onCreate()
{
	// get filepath from user
	//std::string path;
	//std::cout << "Enter Filepath: ";
	//std::cin >> path;
	//bool comments = true;

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
	//m_objModel = OBJLoader::OBJProcess(path, comments);
	//if (m_objModel == nullptr) return false;

	//set shader programs
	//obj
	GLuint vertexShader = ShaderManager::LoadShader("resource/shaders/obj_vertex.glsl", GL_VERTEX_SHADER);
	GLuint fragmentShader = ShaderManager::LoadShader("resource/shaders/obj_fragment.glsl", GL_FRAGMENT_SHADER);
	m_objProgram = ShaderManager::CreateProgram(vertexShader, fragmentShader);
	//ui
	vertexShader = ShaderManager::LoadShader("resource/shaders/vertex.glsl", GL_VERTEX_SHADER);
	fragmentShader = ShaderManager::LoadShader("resource/shaders/fragment.glsl", GL_FRAGMENT_SHADER);
	m_uiProgram = ShaderManager::CreateProgram(vertexShader, fragmentShader);
	m_lineSize = 42;
	m_lines = new Line[m_lineSize];
	for (unsigned int i = 0; i < m_lineSize/2; ++i)
	{
		glm::vec4 color = (i == 10) ? glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) : glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		int j = i * 2;
		m_lines[j].v0.pos = glm::vec4(i - 10, 0.0f, 10.0f, 1.0f);
		m_lines[j].v1.pos = glm::vec4(i - 10, 0.0f, -10.0f, 1.0f);

		m_lines[j].v0.color = color;
		m_lines[j].v1.color = color;

		m_lines[j + 1].v0.pos = glm::vec4(10, 0.0f, i - 10.0f, 1.0f);
		m_lines[j + 1].v1.pos = glm::vec4(-10.f, 0.0f, i - 10.0f, 1.0f);

		m_lines[j + 1].v0.color = color;
		m_lines[j + 1].v1.color = color;
	}

	//generate buffers
	glGenBuffers(1, &m_lineVBO);
	glBufferData(GL_ARRAY_BUFFER, m_lineSize * sizeof(Line), m_lines, GL_STATIC_DRAW);
	return true;
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
	glUniformMatrix4fv(projectionViewMatrixUniformLocation, 1, false, glm::value_ptr(projectionViewMatrix));

	//enable array attribs
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//tell opengl where array is, no of element coponents, data type, normalization
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)nullptr) + 16);
	//draw line grid
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
	glDrawArrays(GL_LINES, 0, m_lineSize *2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//draw OBJ model
	//glBindBuffer(GL_ARRAY_BUFFER, m_objModelBuffer[0]);

	//unbind buffer/arrays and release program
	glDisableVertexAttribArray(0); //position
	glDisableVertexAttribArray(1); //normal
	//glDisableVertexAttribArray(2); //uvcoord
	glUseProgram(0);
}

 void RenderWindow::Destroy() 
{
	//glDeleteBuffers(1, &location);
	ShaderManager::DeleteProgram(m_uiProgram);
}
