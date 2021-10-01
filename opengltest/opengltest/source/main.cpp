#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <fstream>
#include <string>
#include "Utilities.h"

int main()
{
	//Initialise GLFW
	if (!glfwInit())
	{
		return -1;
	}
	//create a windowed mode window and it's OpenGL context
	GLFWwindow* window;
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

	const float vertexPositions[] =
	{
	0.0f, 0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.0f, 1.0f,
	};
	const float vertexColours[] =
	{
	1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	};
	//create shader program
	GLuint uiProgram = CreateProgram();


	while (!glfwWindowShouldClose(window)) {


		//clear backbuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//draw code goes here
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		//specify where our vertex array is, how many components each vertex has,
		//the data type of each component and whether the data is normalised or not
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, vertexPositions);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, vertexColours);
		//draw to the screen
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//swap front and back buffers
		glfwSwapBuffers(window);
		//poll for and process events
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}