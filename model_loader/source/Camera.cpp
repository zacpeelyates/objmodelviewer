#include "Camera.h"
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
void FreeMovement(glm::mat4& a_m4Transform, float a_fDeltaTime, float a_fSpeed, const glm::vec3& a_v3Up)
{
	GLFWwindow* window = glfwGetCurrentContext();

	//get camera transform vectors
	glm::vec4 v4Right = a_m4Transform[0];
	glm::vec4 v4Up = a_m4Transform[1];
	glm::vec4 v4Forward = a_m4Transform[2];
	//get location vectors
	glm::vec4 v4Translation = a_m4Transform[3];
	//set speed variables (shift to sprint)
	float defactoSpeed = a_fDeltaTime * a_fSpeed;
	float framespeed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? defactoSpeed * 2.0f: defactoSpeed;


	if (glfwGetKey(window, 'W') == GLFW_PRESS)
	{		
		v4Translation -= v4Forward * framespeed;
	}
	if (glfwGetKey(window, 'S') == GLFW_PRESS)
	{
		v4Translation += v4Forward * framespeed;
	}
	if (glfwGetKey(window, 'A') == GLFW_PRESS)
	{
		v4Translation -= v4Right * framespeed;
	}
	if (glfwGetKey(window, 'D') == GLFW_PRESS)
	{
		v4Translation += v4Right * framespeed;
	}
	if (glfwGetKey(window, 'Q') == GLFW_PRESS)
	{
		v4Translation += v4Up * framespeed;
	}
	if (glfwGetKey(window, 'E') == GLFW_PRESS)
	{
		v4Translation -= v4Up * framespeed;
	}

	a_m4Transform[3] = v4Translation;

	//rotation
	static bool sbMouse2ButtonDown = false;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		static double sdPrevMouseX = 0;
		static double sdPrevMouseY = 0;
		if (sbMouse2ButtonDown == false)
		{
			sbMouse2ButtonDown = true;
			glfwGetCursorPos(window, &sdPrevMouseX, &sdPrevMouseY);
		}

		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		double dDeltaX = mouseX - sdPrevMouseX;
		double dDeltaY = mouseY - sdPrevMouseY;

		sdPrevMouseX = mouseX;
		sdPrevMouseY = mouseY;


		glm::mat4 m4Temp;
		if (dDeltaY != 0)
		{
			//set temp to pitch
			m4Temp = glm::axisAngleMatrix(v4Right.xyz(), (float)-dDeltaY / 150.0f);
			v4Right = m4Temp * v4Right;
			v4Up = m4Temp * v4Up;
			v4Forward = m4Temp * v4Forward;
		}

		if (dDeltaX != 0)
		{
			//set temp to yaw
			m4Temp = glm::axisAngleMatrix(a_v3Up, (float)-dDeltaX / 150.0f);
			v4Right = m4Temp * v4Right;
			v4Up = m4Temp * v4Up;
			v4Forward = m4Temp * v4Forward;
		}

		a_m4Transform[0] = v4Right;
		a_m4Transform[1] = v4Up;
		a_m4Transform[2] = v4Forward;
	}
	else
	{
		sbMouse2ButtonDown = false;
	}
}