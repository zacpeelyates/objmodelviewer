//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	RenderWindow.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 4/11/21
// Last Edited:  09/01/22
// Brief: Function defenitions for RenderWindow (overrides Applications). Handles tracking and drawing model data with OpenGL
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H
#include <map>
#include <string>
#include <vector>
#include "Application.h"
#include "Event.h"
#include "glm.hpp"

class OBJModel;

class RenderWindow : public Application
{
public:
	RenderWindow();
	virtual ~RenderWindow();
	bool OBJSetup(std::string a_filename);
	//events
	void onWindowResize(WindowResizeEvent* e);

	
protected:
	bool onCreate() override;
	void Update(float deltaTime) override;
	void Draw() override;
	void Destroy() override;
private:
	//skybox
	unsigned int m_skyboxID;
	//matricies
	glm::mat4 m_cameraMatrix;
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
	//programs
	unsigned int m_uiProgram;
	unsigned int m_objProgram;
	unsigned int m_skyboxProgram;
	//vertex buffer objects
	unsigned int m_lineVBO;
	unsigned int m_objModelBuffer[2];
	unsigned int m_skyboxVBO;
	unsigned int m_skyboxVAO;


	//member data structs
	struct Vertex
	{
		glm::vec3 pos; 
		glm::vec4 color;
	};

	struct Line
	{
		Vertex v0;
		Vertex v1;
	};

	struct Triangle
	{
		Vertex v0;
		Vertex v1;
		Vertex v2;
	};

	struct Quad
	{
		Vertex v0;
		Vertex v1;
		Vertex v2;
		Vertex v3;
	};
	struct Polygon
	{
		std::vector<Vertex> points;
	};

	//models
	OBJModel* m_objModel;
	Line* m_lines;
	int m_lineSize;
	std::map<std::string,OBJModel*> m_modelMap;
	//editables
	glm::vec3 m_clearColor;
	glm::vec4 m_lightColor;
	float m_fov;
	

	//input handling
	std::string m_input;

	//verts
	float m_skyboxVertices[108] =
	{
		// positions          
		-2.5f,  2.5f, -2.5f,
		-2.5f, -2.5f, -2.5f,
		 2.5f, -2.5f, -2.5f,

		 2.5f, -2.5f, -2.5f,
		 2.5f,  2.5f, -2.5f,
		-2.5f,  2.5f, -2.5f,

		-2.5f, -2.5f,  2.5f,
		-2.5f, -2.5f, -2.5f,
		-2.5f,  2.5f, -2.5f,

		-2.5f,  2.5f, -2.5f,
		-2.5f,  2.5f,  2.5f,
		-2.5f, -2.5f,  2.5f,

		 2.5f, -2.5f, -2.5f,
		 2.5f, -2.5f,  2.5f,
		 2.5f,  2.5f,  2.5f,

		 2.5f,  2.5f,  2.5f,
		 2.5f,  2.5f, -2.5f,
		 2.5f, -2.5f, -2.5f,

		-2.5f, -2.5f,  2.5f,
		-2.5f,  2.5f,  2.5f,
		 2.5f,  2.5f,  2.5f,

		 2.5f,  2.5f,  2.5f,
		 2.5f, -2.5f,  2.5f,
		-2.5f, -2.5f,  2.5f,

		-2.5f,  2.5f, -2.5f,
		 2.5f,  2.5f, -2.5f,
		 2.5f,  2.5f,  2.5f,

		 2.5f,  2.5f,  2.5f,
		-2.5f,  2.5f,  2.5f,
		-2.5f,  2.5f, -2.5f,

		-2.5f, -2.5f, -2.5f,
		-2.5f, -2.5f,  2.5f,
		 2.5f, -2.5f, -2.5f,

		 2.5f, -2.5f, -2.5f,
		-2.5f, -2.5f,  2.5f,
		 2.5f, -2.5f,  2.5f
	};

};
#endif // RENDERWINDOW_H
