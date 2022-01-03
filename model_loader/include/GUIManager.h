#pragma once
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <string>
#include <vector>

class GUIManager 
{

public:
	static GUIManager* GetInstance();
	static GUIManager* CreateInstance();
	static void DestroyInstance();
	bool Init(GLFWwindow* window);
	void NewFrame();
	void Render();
	void ShowFrameData();
	void SetupNextWindow();
	bool ShowFileLoader(std::string& input);
	bool ShowLoadedFileList(std::vector<std::string> loadedFiles, std::string& selectedFile);
	
	

private:
	GUIManager();
	~GUIManager();
	static GUIManager* m_instance;
	static const int PADDING;
	int m_yOffset;
	int m_corner;
	bool m_bShow;
	std::string m_buffer;
	

};

#define IMGUI_STATIC_INFO_FLAGS ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav
