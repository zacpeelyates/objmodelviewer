#include "RenderWindow.h"
#include "ShaderManager.h"
#include "Utilities.h"
#include "obj_Loader.h"
#include "TextureManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <ext.hpp> //glm ext
#include <iostream>

RenderWindow::RenderWindow() {};
RenderWindow::~RenderWindow() {};

bool RenderWindow::onCreate()
{
	// get filepath from user
	std::string path;
	bool comments = true;

	std::cout << "Enter Filename: ";
	std::getline(std::cin, path);
	path = "./resource/obj_models/" + path;

	m_objModel = OBJLoader::OBJProcess(path, comments);
	if (m_objModel == nullptr) return false;

	//handle textures
	TextureManager* pTexM = TextureManager::CreateInstance();
	for (unsigned int i = 0; i < m_objModel->GetMaterialCount(); ++i) 
	{
		OBJMaterial* currentMaterial = m_objModel->GetMaterial(i);
		for (int j = 0; j < OBJMaterial::TextureTypes_Count; ++j) 
		{
			if (currentMaterial->textureFileNames[j].size() > 0)
			{
				currentMaterial->textureIDs[j] = pTexM->LoadTexture(currentMaterial->textureFileNames[j].c_str());
			}
		}
	}

	

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
		m_lines[j].v0.pos = glm::vec3(i - 10.0f, 0.0f, 10.0f);
		m_lines[j].v1.pos = glm::vec3(i - 10.0f, 0.0f, -10.0f);

		m_lines[j].v0.color = color;
		m_lines[j].v1.color = color;

		m_lines[j + 1].v0.pos = glm::vec3(10.0f, 0.0f, i - 10.0f);
		m_lines[j + 1].v1.pos = glm::vec3(-10.0f, 0.0f, i - 10.0f);

		m_lines[j + 1].v0.color = color;
		m_lines[j + 1].v1.color = color;
	}

	//generate buffers
	glGenBuffers(1, &m_lineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
	glBufferData(GL_ARRAY_BUFFER, m_lineSize * sizeof(Line), m_lines, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(2, m_objModelBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_objModelBuffer[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_objModelBuffer[1]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
	int projectionViewMatrixUniformLocation = glGetUniformLocation(m_uiProgram, "ProjectionViewMatrix");
	//send pointer to location of matrix 
	glUniformMatrix4fv(projectionViewMatrixUniformLocation, 1, false, glm::value_ptr(projectionViewMatrix));
	//enable array attribs
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//tell opengl where array is, no of element components, data type, normalization
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)nullptr) + sizeof(float)*3); //x3 because vec3 used for position data
	//draw line grid
	glDrawArrays(GL_LINES, 0, (m_lineSize *2) + 10);

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//draw OBJ model
	glUseProgram(m_objProgram);
	projectionViewMatrixUniformLocation = glGetUniformLocation(m_objProgram, "ProjectionViewMatrix");
	//send pointer to location of matrix 

	//light position 
	glm::vec3 lightPos;
	double time = glfwGetTime();
	lightPos.x = sin(time) * 5.0f;
	lightPos.y = sin(time / 2.0f);
	lightPos.z = 10.0f;

	glUniformMatrix4fv(projectionViewMatrixUniformLocation, 1, false, glm::value_ptr(projectionViewMatrix));
	for (int i = 0; i < m_objModel->GetMeshCount(); ++i) 
	{
		int ModelMatrixUniformLocation = glGetUniformLocation(m_objProgram, "ModelMatrix");
		glUniformMatrix4fv(ModelMatrixUniformLocation, 1, false, glm::value_ptr(m_objModel->GetWorldMatrix()));

		int cameraPositionUniformLocation = glGetUniformLocation(m_objProgram, "camPos");
		glUniform3fv(cameraPositionUniformLocation, 1, glm::value_ptr(m_cameraMatrix[3]));
		OBJMesh* currentMesh = m_objModel->GetMesh(i);
		int kA_location = glGetUniformLocation(m_objProgram, "kA");
		int kD_location = glGetUniformLocation(m_objProgram, "kD");
		int kS_location = glGetUniformLocation(m_objProgram, "kS");
		int nS_location = glGetUniformLocation(m_objProgram, "nS");
		int lightPos_location = glGetUniformLocation(m_objProgram, "lightPos");
		glUniform3fv(lightPos_location, 1, glm::value_ptr(lightPos));


		OBJMaterial* currentMaterial = currentMesh->m_activeMaterial;
		bool bUseNormal = false;
		bool bUseSpecular = false;
		bool bUseDiffuse = false;

		if (currentMaterial != nullptr)
		{
			glUniform3fv(kA_location, 1, glm::value_ptr(currentMaterial->GetAmbience()));
			glUniform3fv(kD_location, 1, glm::value_ptr(currentMaterial->GetDiffuse()));
			glUniform3fv(kS_location, 1, glm::value_ptr(currentMaterial->GetSpecular()));
			glUniform1f(nS_location, currentMaterial->GetSpecularExponent());


			//textures
			int TextureUniformLocation;
			if (!currentMaterial->textureFileNames[OBJMaterial::DiffuseTexture].empty()) {
				//diffuse
				bUseDiffuse = true;
				TextureUniformLocation = glGetUniformLocation(m_objProgram, "DiffuseTexture");
				glUniform1i(TextureUniformLocation, 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, currentMaterial->textureIDs[OBJMaterial::DiffuseTexture]);
			}

			if (!currentMaterial->textureFileNames[OBJMaterial::SpecularTexture].empty()) {
				//specular
				bUseSpecular = true;
				TextureUniformLocation = glGetUniformLocation(m_objProgram, "SpecularTexture");
				glUniform1i(TextureUniformLocation, 1);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, currentMaterial->textureIDs[OBJMaterial::SpecularTexture]);
			}

			if (!currentMaterial->textureFileNames[OBJMaterial::NormalTexture].empty()) {
				//normal
				bUseNormal = true;
				TextureUniformLocation = glGetUniformLocation(m_objProgram, "NormalTexture");
				glUniform1i(TextureUniformLocation, 2);
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, currentMaterial->textureIDs[OBJMaterial::NormalTexture]);
			}

		}
		else
		{
			//use default lighting
			glUniform3fv(kA_location, 1, glm::value_ptr(glm::vec3(0.25f)));
			glUniform3fv(kD_location, 1, glm::value_ptr(glm::vec3(0.25f)));
			glUniform3fv(kS_location, 1, glm::value_ptr(glm::vec3(0.25f)));
			glUniform1f(nS_location, 1.0f);
		}
		int TextureCheckUniformLocation = glGetUniformLocation(m_objProgram, "useNormal");
		glUniform1i(TextureCheckUniformLocation, bUseNormal);
		TextureCheckUniformLocation = glGetUniformLocation(m_objProgram, "useSpecular");
		glUniform1i(TextureCheckUniformLocation, bUseSpecular);
		TextureCheckUniformLocation = glGetUniformLocation(m_objProgram, "useDiffuse");
		glUniform1i(TextureCheckUniformLocation, bUseDiffuse);

		//bind buffers
		glBindBuffer(GL_ARRAY_BUFFER, m_objModelBuffer[0]);
		glBufferData(GL_ARRAY_BUFFER, currentMesh->m_verts.size() * sizeof(OBJVertex), currentMesh->m_verts.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); //position
		glEnableVertexAttribArray(1); //normal
		glEnableVertexAttribArray(2); //UVs

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), ((char*)0) + OBJVertex::Offsets::POS); //vec3 pos
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), ((char*)0) + OBJVertex::Offsets::NORMAL); //vec3 normal
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), ((char*)0) + OBJVertex::Offsets::UVCOORD); //vec2 UVs

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_objModelBuffer[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, currentMesh->m_indicies.size() * sizeof(unsigned int), currentMesh->m_indicies.data(), GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, currentMesh->m_indicies.size(), GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	//unbind buffer/arrays and release program
	glDisableVertexAttribArray(0); //position
	glDisableVertexAttribArray(1); //normal
	glDisableVertexAttribArray(2); //uvcoord
	glUseProgram(0);
}

 void RenderWindow::Destroy() 
{
	delete m_objModel;
	delete[] m_lines;
	glDeleteBuffers(1, &m_lineVBO);
	ShaderManager::DestroyInstance();
	TextureManager::DestroyInstance();
}
