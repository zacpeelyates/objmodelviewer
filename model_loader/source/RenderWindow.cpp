#include "RenderWindow.h"
#include "ShaderManager.h"
#include "Utilities.h"
#include "obj_Loader.h"
#include "TextureManager.h"
#include "Dispatcher.h"
#include "GUIManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <ext.hpp> //glm ext
#include <iostream>


RenderWindow::RenderWindow() { m_fov = 60; };
RenderWindow::~RenderWindow() {}

void RenderWindow::onWindowResize(WindowResizeEvent* e)
{
	if (e->GetWidth() > 0 && e->GetHeight() > 0) { //fixed crash when making window tiny
		m_windowWidth = e->GetWidth();
		m_windowHeight = e->GetHeight();
		e->Handled();
		std::cout << "Window Resize Event" << std::endl << "w: " << m_windowWidth << " h: " << m_windowHeight << std::endl;
		m_projectionMatrix = glm::perspective(glm::radians(m_fov), (float)(m_windowWidth / m_windowHeight), 0.1f, 1000.0f);
		glViewport(0, 0, m_windowWidth, m_windowHeight);
	}
};



bool RenderWindow::onCreate()
{
	Dispatcher* dp = Dispatcher::GetInstance();
	if (dp != nullptr) 
	{
		dp->Subscribe(this, &RenderWindow::onWindowResize);
	}


	TextureManager* pTexM = TextureManager::GetInstance();

	//handle skybox
	std::string path = "./resource/skyboxes/";
	std::string skyboxName = "skybox_1"; //testing, should be variable
	m_skyboxID = pTexM->LoadTexture((path + skyboxName).c_str(), true);

	//setup clear color, depth test, culling
	m_clearColor = glm::vec3(0.95f, 0.45f, 0.75f);
	glClearColor(m_clearColor.x,m_clearColor.y,m_clearColor.z,1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearDepth(1.0f);

	//set viewport
	glViewport(0, 0, m_windowWidth, m_windowHeight);

	//create matricies
	m_cameraMatrix = glm::inverse(glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	m_projectionMatrix = glm::perspective(glm::radians(m_fov), (float)(m_windowWidth / m_windowHeight), 0.1f, 1000.0f);


	//lighting
	m_lightColor = glm::vec4(1, 1, 1,1);
	//set shader programs
	//obj
	GLuint vertexShader = ShaderManager::LoadShader("resource/shaders/obj_vertex.glsl", GL_VERTEX_SHADER);
	GLuint fragmentShader = ShaderManager::LoadShader("resource/shaders/obj_fragment.glsl", GL_FRAGMENT_SHADER);
	m_objProgram = ShaderManager::CreateProgram(vertexShader, fragmentShader);
	
	//skybox
	vertexShader = ShaderManager::LoadShader("resource/shaders/skybox_vertex.glsl", GL_VERTEX_SHADER);
    fragmentShader = ShaderManager::LoadShader("resource/shaders/skybox_fragment.glsl", GL_FRAGMENT_SHADER);
	m_skyboxProgram = ShaderManager::CreateProgram(vertexShader, fragmentShader);

	//ui
	vertexShader = ShaderManager::LoadShader("resource/shaders/ui_vertex.glsl", GL_VERTEX_SHADER);
	fragmentShader = ShaderManager::LoadShader("resource/shaders/ui_fragment.glsl", GL_FRAGMENT_SHADER);
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
	//lines
	glGenBuffers(1, &m_lineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
	glBufferData(GL_ARRAY_BUFFER, m_lineSize * sizeof(Line), m_lines, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//skybox
	glGenVertexArrays(1, &m_skyboxVAO);
	glBindVertexArray(m_skyboxVAO);
	glGenBuffers(1, &m_skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_skyboxVertices), m_skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	//model
	glGenBuffers(2, m_objModelBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_objModelBuffer[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_objModelBuffer[1]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}
 

void RenderWindow::Update(float deltaTime)
{
	//Camera
	Utilities::FreeMovement(m_cameraMatrix,deltaTime);

	//GUI
	GUIManager* gui = GUIManager::GetInstance();
	std::string filename;

	if (gui->ShowFileLoader(filename))
	{
		auto iter = m_modelMap.find(filename);
		if (iter == m_modelMap.end())
		{
			if (OBJSetup(filename)) 
			{
				m_modelMap.emplace(filename, m_objModel);
			}
		}
		else 
		{
			m_objModel = iter->second;
		}
	}
	std::vector<std::string> keys;
	for (auto const& iter : m_modelMap) 
	{
		keys.push_back(iter.first);
	}
	if (!keys.empty()) {
		if (gui->ShowLoadedFileList(keys, filename))
		{
			m_objModel = m_modelMap.find(filename)->second;
		}
	}

	if (gui->ShowColorEditor(&m_clearColor.x, "Clear Color"))
	{
		glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, 1.0f);
	}
	
	gui->ShowColorEditor(&m_lightColor.x, "Light Color",true);

	if (m_objModel != nullptr) 
	{
		float* pModelMatrix = (float*)glm::value_ptr(m_objModel->GetWorldMatrix()); //get model matrix
		if (gui->ShowMatrixEditor(pModelMatrix, (const float*)glm::value_ptr(m_viewMatrix), (const float*)glm::value_ptr(m_projectionMatrix))) //edit model matrix
		{
			m_objModel->SetWorldMatrix(glm::make_mat4<float>(pModelMatrix)); //set new model matrix
		}
	}

	if (gui->ShowSlider(&m_fov, 1, 179, "FOV")) 
	{
		m_projectionMatrix = glm::perspective(glm::radians(m_fov), (float)(m_windowWidth / m_windowHeight), 0.1f, 1000.0f);
		glViewport(0, 0, m_windowWidth, m_windowHeight);
	}


}

bool RenderWindow::OBJSetup(std::string a_filename)
{
	bool comments = true;
	a_filename = "./resource/obj_models/" + a_filename;
	m_objModel = OBJLoader::OBJProcess(a_filename, comments);
	if (m_objModel == nullptr) return false;

	//handle textures
	TextureManager* pTexM = TextureManager::CreateInstance();
	for (unsigned int i = 0; i < m_objModel->GetMaterialCount(); ++i)
	{
		OBJMaterial* currentMaterial = m_objModel->GetMaterial(i);
		for (int j = 0; j < OBJMaterial::TextureTypes_Count; ++j)
		{
			if (!currentMaterial->textureFileNames[j].empty())
			{
				currentMaterial->textureIDs[j] = pTexM->LoadTexture(currentMaterial->textureFileNames[j].c_str());
			}
		}
	}
}

void RenderWindow::Draw() 
{
	//clear backbuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//enable shaders
	glUseProgram(m_uiProgram);
	//get view matrix from camera matrix in world space
	m_viewMatrix = glm::inverse(m_cameraMatrix);
	glm::mat4 projectionViewMatrix = m_projectionMatrix * m_viewMatrix;
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
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)nullptr + sizeof(float)*3); //x3 because vec3 used for position data
	//draw line grid
	glDrawArrays(GL_LINES, 0, (m_lineSize *2) + 10);

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//light position 
	glm::vec3 lightPos;
	double time = glfwGetTime();
	lightPos.x = sin(time) * 5.0f;
	lightPos.y = sin(time / 2.0f);
	lightPos.z = 10.0f;

	//draw OBJ model
	if (m_objModel != nullptr)
	{
		glUseProgram(m_objProgram);
		projectionViewMatrixUniformLocation = glGetUniformLocation(m_objProgram, "ProjectionViewMatrix");
		//send pointer to location of matrix 

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
			int lightColor_location = glGetUniformLocation(m_objProgram, "lightColor");
			glUniform4fv(lightColor_location, 1, glm::value_ptr(m_lightColor));


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

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), (char*)0 + OBJVertex::Offsets::POS); //vec3 pos
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), (char*)0 + OBJVertex::Offsets::NORMAL); //vec3 normal
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), (char*)0 + OBJVertex::Offsets::UVCOORD); //vec2 UVs

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_objModelBuffer[1]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, currentMesh->m_indicies.size() * sizeof(unsigned int), currentMesh->m_indicies.data(), GL_STATIC_DRAW);
			glDrawElements(GL_TRIANGLES, currentMesh->m_indicies.size(), GL_UNSIGNED_INT, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

		}

		//unbind buffer/arrays 
		glDisableVertexAttribArray(0); //position
		glDisableVertexAttribArray(1); //normal
		glDisableVertexAttribArray(2); //uvcoord
	}

	//draw skybox
	glDepthFunc(GL_LEQUAL);
	glUseProgram(m_skyboxProgram);
	projectionViewMatrix = glm::mat4(glm::mat3(projectionViewMatrix)); //remove translation
	projectionViewMatrixUniformLocation = glGetUniformLocation(m_skyboxProgram, "ProjectionViewMatrix");
	glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
	glBindVertexArray(m_skyboxVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); //vec3 UVCoords in shader
	int SkyboxTextureUniformLocation = glGetUniformLocation(m_skyboxProgram, "skybox");
	glUniform1i(SkyboxTextureUniformLocation,0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP,m_skyboxID);
	glDrawArrays(GL_TRIANGLES, 0, 36); //108 floats in skyboxverticies/3 (each vertex is 3 floats)
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); //set to default depth test
	//release program
	glUseProgram(0);
}

 void RenderWindow::Destroy() 
{
	delete m_objModel;
	delete[] m_lines;
	glDeleteVertexArrays(1, &m_skyboxVAO);
	glDeleteBuffers(1, &m_lineVBO);
	glDeleteBuffers(1, &m_skyboxVBO);
	glDeleteBuffers(2, &m_objModelBuffer[0]);
	ShaderManager::DestroyInstance();
	TextureManager::DestroyInstance();
	glfwTerminate();
}
