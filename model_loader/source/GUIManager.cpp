#include "GUIManager.h"
#include <imgui_stdlib.h>


const int GUIManager::PADDING = 10;
GUIManager* GUIManager::m_instance = nullptr;

GUIManager* GUIManager::GetInstance()
{
	return m_instance == nullptr ? CreateInstance() : m_instance;
}

GUIManager* GUIManager::CreateInstance()
{
	if (m_instance == nullptr) m_instance = new GUIManager();
	return m_instance;
}

void GUIManager::DestroyInstance()
{
	if (m_instance != nullptr)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

GUIManager::GUIManager()
{
	m_yOffset = 0;
	m_corner = 0;
	m_bShow = false;
}

GUIManager::~GUIManager()
{
	//Imgui cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}


bool GUIManager::Init(GLFWwindow* window)
{
	//imgui setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	if(!ImGui_ImplGlfw_InitForOpenGL(window, true)) return false;
	if(!ImGui_ImplOpenGL3_Init("#version 400")) return false;
	return true;
}

void GUIManager::NewFrame()
{
	//new IMGUI frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	m_yOffset = PADDING;
}

void GUIManager::Render()
{
	//IMUI render
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUIManager::ShowFrameData()
{
	SetupNextWindow();
	if (ImGui::Begin("Frame Data", &m_bShow, IMGUI_STATIC_INFO_FLAGS))
	{
		m_yOffset += PADDING + ImGui::GetWindowHeight();
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("FPS: %.1f (Frame Delay: %.3f ms)", io.Framerate, 1000 / io.Framerate);
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

void GUIManager::SetupNextWindow() 
{
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 windowPos = ImVec2((m_corner & 1) ? io.DisplaySize.x - PADDING : PADDING, (m_corner & 2) ? io.DisplaySize.y - m_yOffset : m_yOffset);
	ImVec2 windowPivot = ImVec2((m_corner & 1) ? 1.0f : 0.0f, (m_corner & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always, windowPivot);
	ImGui::SetNextWindowBgAlpha(0.4f);
}



bool GUIManager::ShowFileLoader(std::string& input) 
{
	bool b = false;
	ImGuiIO& io = ImGui::GetIO();
	SetupNextWindow();
	if (ImGui::Begin("Load Model", &m_bShow, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::InputText("Filename", &m_buffer);
		if (!m_buffer.empty())
		{
			input = m_buffer;
		}

		b = ImGui::Button("Load"), ImVec2(50, 75);
		m_yOffset += PADDING + ImGui::GetWindowHeight();
		
	}
	ImGui::End();
	return b;
}

bool GUIManager::ShowLoadedFileList(std::vector<std::string> loadedFiles, std::string& selectedFile)
{
	bool b = false;
	ImGuiIO& io = ImGui::GetIO();
	SetupNextWindow();
	if (ImGui::Begin("Loaded Files", &m_bShow, IMGUI_STATIC_INFO_FLAGS))
	{
		for (int i = 0; i < loadedFiles.size(); ++i) 
		{
			bool selected = false;
			if (ImGui::Selectable(loadedFiles[i].c_str(), &selected, ImGuiSelectableFlags_DontClosePopups,ImVec2(200,15))) 
			{
				if (selected) 
				{
					selectedFile = loadedFiles[i];
					b = true;
				}
			}
		}
		m_yOffset += PADDING + ImGui::GetWindowHeight();
	}
	ImGui::End();
	return b;
	
}

bool GUIManager::ShowColorEditor(float* firstElement, std::string title, bool alpha)
{
	bool b = false;
	SetupNextWindow();
	if (ImGui::Begin(title.c_str(),&m_bShow, IMGUI_STATIC_INFO_FLAGS))
	{	
		b = alpha ? ImGui::ColorEdit4(title.c_str(), firstElement) : ImGui::ColorEdit3(title.c_str(), firstElement);
		m_yOffset += PADDING + ImGui::GetWindowHeight();
	}
	ImGui::End();
	return b;
}



