#include "Application.h"
#include "Utilities.h"
#include "ShaderManager.h"
#include "Dispatcher.h"
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

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
	//imgui setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 400");


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
			//new IMGUI frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			//ImGui Info Window
			showFrameData(true);

			float deltaTime = Utilities::TimerTick();
			Update(deltaTime);
			Draw();

			//IMUI render
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(m_window);
			glfwPollEvents();

		} while (m_running && glfwWindowShouldClose(m_window) == 0);
		Destroy();
	}
	ShaderManager::DestroyInstance();
	glfwDestroyWindow(m_window);
	glfwTerminate();
	//Imgui cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	Dispatcher::DestroyInstance();
}

#define IMGUI_STATIC_INFO_FLAGS ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav


void Application::showFrameData(bool bShow)
{
	const float DIST = 10.0F;
	static int corner = 0;
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 windowPos = ImVec2((corner & 1) ? io.DisplaySize.x - DIST : DIST, (corner & 2) ? io.DisplaySize.y - DIST : DIST);
	ImVec2 windowPivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);

	ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always, windowPivot);
	ImGui::SetNextWindowBgAlpha(0.4f);

	if (ImGui::Begin("Frame Data", &bShow, IMGUI_STATIC_INFO_FLAGS))
	{
		ImGui::Text("FPS: %.1f (Frame Delay: %.3f ms)", io.Framerate, 1000/io.Framerate);
		if (ImGui::IsMousePosValid())
		{
			ImGui::Text("MousePos: %1.f,%1.f", io.MousePos.x, io.MousePos.y);
		} 
		else
		{
			ImGui::Text("MousePos: <offscreen>");
		}
	}
	ImGui::End();
}
