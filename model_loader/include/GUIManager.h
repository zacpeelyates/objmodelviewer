#pragma once
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <string>
#include <vector>
#include <ImGuizmo.h> //https://github.com/CedricGuillemet/ImGuizmo -- extentions for ImGUI

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
	bool ShowColorEditor(float* firstElement, std::string title = "ColorEditor", bool alpha = false);

	bool ShowSlider(float* valueToEdit, float a_min, float a_max, std::string title);

	bool ShowMatrixEditor(float matrixToEdit[16], const float viewMatrix[16], const float projectionMatrix[16]);

	
	

private:
	GUIManager();
	~GUIManager();
	static GUIManager* m_instance;
	static const int PADDING;
	int m_yOffset;
	int m_corner;
	bool m_bShow;

	std::string m_inputBuffer;
	
	static ImGuizmo::OPERATION m_operation;
	static ImGuizmo::MODE m_mode;

};

#define IMGUI_STATIC_INFO_FLAGS ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav
