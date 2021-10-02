#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Utilities.h"
#include "Camera.h"


constexpr float WINDOW_WIDTH = 640.0f;
constexpr float WINDOW_HEIGHT = 480.0f;
constexpr float TIMESTEP = 1.0f/60.0f;
constexpr float BASESPEED = 0.5f;

struct Vertex
{
	glm::vec4 pos;
	glm::vec4 color;
};

struct Line
{
	Vertex start;
	Vertex end;
};

int main()
{
	glm::mat4 cameraMatrix = glm::inverse(glm::lookAt(glm::vec3(10,10,10),glm::vec3(0,0,0),glm::vec3(0, 1, 0)));

	glm::mat4 projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, (float)(WINDOW_WIDTH / WINDOW_HEIGHT),0.1f, 1000.0f);

	//Initialise GLFW
	if (!glfwInit())
	{
		return -1;
	}
	//create a windowed mode window and it's OpenGL context
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Testing", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	//make the window's context current
	glfwMakeContextCurrent(window);
	//loop until the user closes the window
	if (!gladLoadGL()) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.95f, 0.45f, 0.75f, 1.0f);
	//create shader program
	GLuint uiProgram = CreateProgram();
	glm::mat4 defaultCameraMatrix = cameraMatrix; //todo add reset button
	glUseProgram(uiProgram);

	int gridSize = 21;
	Line* lines = new Line[gridSize * 2];
	for (int i = 0; i < gridSize; ++i)
	{
		int j = i * 2;
		lines[j].start.pos = glm::vec4(-10 + i, 0.0f, 10.0f, 1.0f);
		lines[j].start.color = (i == 10) ? glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) : glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		lines[j].end.pos = glm::vec4(-10 + i, 0.0f, -10.0f, 1.0f);
		lines[j].end.color = (i == 10) ? glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) : glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		lines[j+1].start.pos = glm::vec4(10.0f, 0.0f, -10.0f + i, 1.0f);
		lines[j+1].start.color = (i == 10) ? glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) : glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		lines[j+1].end.pos = glm::vec4(-10, 0.0f, -10.0f + i, 1.0f);
		lines[j+1].end.color = (i == 10) ? glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) : glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	unsigned int lineVBO;
	glGenBuffers(1, &lineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);

	glBufferData(GL_ARRAY_BUFFER, gridSize * 2 * sizeof(Line), lines, GL_STATIC_DRAW);
	delete[] lines;
	

	while (!glfwWindowShouldClose(window)) {
	
		FreeMovement(cameraMatrix, TIMESTEP, BASESPEED);

		//transform matricies 
		glm::mat4 viewMatrix = glm::inverse(cameraMatrix);
		glm::mat4 projectionViewMatrix = projectionMatrix * viewMatrix;
		int projectionViewMatrixUniformLocation = glGetUniformLocation(uiProgram, "ProjectionViewMatrix");
		glUniformMatrix4fv(projectionViewMatrixUniformLocation, 1, false, glm::value_ptr(projectionViewMatrix));
		
		//clear backbuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//draw code goes here
		glEnableVertexAttribArray(0); 
		glEnableVertexAttribArray(1);
		//specify where our vertex array is, how many components each vertex has,
		//the data type of each component and whether the data is normalized or not
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)nullptr+16);
		glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
		//draw to the screen
		glDrawArrays(GL_LINES, 0, gridSize * 4);
		//swap front and back buffers
		glfwSwapBuffers(window);
		//poll for and process events
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}