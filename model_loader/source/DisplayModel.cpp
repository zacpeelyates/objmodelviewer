#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Camera.h"
#include "ShaderManager.h"
#include "DisplayModel.h"


constexpr float WINDOW_WIDTH = 640.0f;
constexpr float WINDOW_HEIGHT = 480.0f;

void resizeWindow(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

bool draw()
{
	//Initialise GLFW
	if (!glfwInit())
	{
		return false;
	}
	//create a windowed mode window and it's OpenGL context
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Testing", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return false;
	}

	//make the window's context current
	glfwMakeContextCurrent(window);
	//loop until the user closes the window
	if (!gladLoadGL()) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	//set program

	GLuint vertexShader = ShaderManager::LoadShader("resource/shaders/vertex.glsl", GL_VERTEX_SHADER);
	GLuint fragmentShader = ShaderManager::LoadShader("resource/shaders/fragment.glsl", GL_FRAGMENT_SHADER);
	GLuint uiProgram = ShaderManager::CreateProgram(vertexShader,fragmentShader);

	//set viewport
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	//resize viewport if window changes size via callback
	glfwSetFramebufferSizeCallback(window, resizeWindow);

	//set up
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.95f, 0.45f, 0.75f, 1.0f);
	glClearDepth(1.0);


	//set matrix values 
	glm::mat4 cameraMatrix = glm::inverse(glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	glm::mat4 projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, (float)(WINDOW_WIDTH / WINDOW_HEIGHT), 0.1f, 1000.0f);

	//glvertexattribpointer:
		//1. which vertex attrib we are configuring (defined in vertex.glsl -- 0 = position, 1 = color)
		//2. size of vertex attrib (vec3 = 3)
		//3. data type (GL_FLOAT, usually)
		//4. normalize data boolean
		//5. space between vertex attribs (vec3 would be 3 * sizeof(float))
		//6. offset of position data from void*

	while (!glfwWindowShouldClose(window)) {
	
		FreeMovement(cameraMatrix);
		//transform matrices 
		glm::mat4 viewMatrix = glm::inverse(cameraMatrix);
		glm::mat4 projectionViewMatrix = projectionMatrix * viewMatrix;
		int projectionViewMatrixUniformLocation = glGetUniformLocation(uiProgram, "ProjectionViewMatrix");
		glUniformMatrix4fv(projectionViewMatrixUniformLocation, 1, false, glm::value_ptr(projectionViewMatrix));
		
		//clear backbuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//DRAW HERE
		//glarraybuffer: 1. buffer type to copy into 2.size of data in bytes, 3. actual data, 4. draw type options
		

		//CLEANUP / LOOP END
		//swap front and back buffers
		glfwSwapBuffers(window);
		//poll for and process events
		glfwPollEvents();

	}
	glfwTerminate();
	return true;
}