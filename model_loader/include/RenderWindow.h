#include "Application.h"
#include "Event.h"
#include <glm.hpp>
#include <vector>

class OBJModel;

class RenderWindow : public Application
{
public:
	RenderWindow();
	virtual ~RenderWindow();
	//events
	void onWindowResize(WindowResizeEvent* e);
protected:
	virtual bool onCreate();
	virtual void Update(float deltaTime);
	virtual void Draw();
	virtual void Destroy();
private:
	//skybox
	unsigned int m_skyboxID;
	//matricies
	glm::mat4 m_cameraMatrix;
	glm::mat4 m_projectionMatrix;
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

	float m_skyboxVertices[108] = {
		//positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

};