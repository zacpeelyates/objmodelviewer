#include "Application.h"
#include "Utilities.h"
#include "ShaderManager.h"
#include "Dispatcher.h"
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "GUIManager.h"

bool Application::Create(const char* a_appName, unsigned int a_windowWidth, unsigned int a_windowHeight, bool a_fullscreen)
{
	if (!glfwInit()) return false;
	m_windowWidth = a_windowWidth;
	m_windowHeight = a_windowHeight;
	m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, a_appName, (a_fullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr);
	if (!m_window)
	{
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_window);

	if (!gladLoadGL())
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return false;
	}
	//get version
	std::cout << glGetString(GL_VERSION) << std::endl;

	Dispatcher::CreateInstance();
	GUIManager* gui = GUIManager::CreateInstance();
	gui->Init(m_window);


	//callbacks
	glfwSetWindowSizeCallback(m_window, [](GLFWwindow*, int width, int height)
		{
			Dispatcher* dp = Dispatcher::GetInstance();
			if (dp != nullptr)
			{
				dp->Publish(new WindowResizeEvent(width, height), true);
			}
		});

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
	
	if (Create(a_name, a_width, a_height, a_fullscreen))
	{
		Utilities::TimerReset();
		m_running = true;
		do
		{
			GUIManager* gui = GUIManager::GetInstance();
			gui->NewFrame();

			//ImGui Info Window
			gui->ShowFrameData();

			float deltaTime = Utilities::TimerTick();
			Update(deltaTime);
			Draw();

			gui->Render();

			glfwSwapBuffers(m_window);
			glfwPollEvents();

		} while (m_running && glfwWindowShouldClose(m_window) == 0);
		Destroy();
	}
	ShaderManager::DestroyInstance();
	glfwDestroyWindow(m_window);
	glfwTerminate();
	GUIManager::DestroyInstance();
	Dispatcher::DestroyInstance();
}
