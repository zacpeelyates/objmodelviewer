#include "Application.h"
#include "Utilities.h"
#include "ShaderManager.h"





#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>


bool Application::Create(const char* a_appName, unsigned int a_windowWidth, unsigned int a_windowHeight, bool a_fullscreen)
{
	if (!glfwInit()) return false;
	m_windowWidth = a_windowWidth;
	m_windowHeight = a_windowHeight;
	m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, a_appName, (a_fullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr);
	if(!m_window)
	{
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_window);
	
	if(!gladLoadGL())
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return false;
	}
	//get version
	int major = glfwGetWindowAttrib(m_window, GLFW_VERSION_MAJOR); 
	int minor = glfwGetWindowAttrib(m_window, GLFW_VERSION_MINOR);
	int revision = glfwGetWindowAttrib(m_window, GLFW_VERSION_REVISION);

	std::cout << "OpenGL Version " << major << "." << minor << "." << revision << std::endl;
	bool result = onCreate();
	if (!result)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
	return result;
}

void Application::Run(const char* a_name, unsigned int a_width, unsigned int a_height, bool a_fullscreen)
{
	if(Create(a_name,a_width,a_height,a_fullscreen))
	{
		Utilities::TimerReset();
		m_running = true;
		do 
		{
			float deltaTime = Utilities::TimerTick();
			Update(deltaTime);
			Draw();
			glfwSwapBuffers(m_window);
			glfwPollEvents();
	
		} while (m_running && glfwWindowShouldClose(m_window) == 0);
		Destroy();
	}
	ShaderManager::DestroyInstance();
	glfwDestroyWindow(m_window);
	glfwTerminate();
}