#include "Input.h"
#include <iostream>
#include "Renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace OpenGLRenderer {

	// Static variables
	std::unordered_map<uint32_t, uint8_t> Input::Keys;
	std::unordered_map<uint8_t, uint8_t> Input::MouseButtons;
	std::unordered_map<uint8_t, uint8_t> Input::MouseButtonsLast;
	Vec2 Input::MousePos;

	void OpenGLRenderer::Input::Setup(GLFWwindow* window)
	{
		// Set Input Callbacks
		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				Input::Keys[key] = action;
			});
		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
			{
				Input::MouseButtons[button] = action;
			});
		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
			{
				// GLFW defines mouse pos, where (0, 0) is the top left of the window and going down increases y-pos
				// This changes (0, 0) to the bottom left and going up increases y-pos
				int height = 0;
				glfwGetWindowSize(window, NULL, &height);
				Input::MousePos = Vec2(xpos, height-ypos);
			});
	}

	void Input::Update()
	{
		// If button is held for more than one frame, changes from GLFW_PRESS to GLFW_REPEAT
		for (auto& button : Input::MouseButtonsLast)
		{
			if (button.second == GLFW_PRESS)
			{
				if (MouseButtons[button.first])
					MouseButtons[button.first] = GLFW_REPEAT;
				else
					MouseButtons[button.first] = GLFW_RELEASE;
			}
		}
		for (auto& button : Input::MouseButtons)
		{
			MouseButtonsLast[button.first] = button.second;
		}
	}

	uint8_t Input::GetKeyState(uint32_t key)
	{
		return Keys[key];
	}
	uint8_t Input::GetMouseButtonState(uint32_t button)
	{
		return MouseButtons[button];
	}
	Vec2 Input::GetMousePos()
	{
		return MousePos;
	}
}