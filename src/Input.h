#pragma once
#include "InputKeys.h"
#include <unordered_map>
#include "Vector.h"
#include <stdint.h>

struct GLFWwindow;

namespace OpenGLRenderer {

	// Get Input from Peripherals
	class Input
	{
	public:
		// Setup input
		static void Setup(GLFWwindow* window);
		// Update input
		static void Update();

		// Get state of key or mouse button
		// Can be GLFW_RELEASE (not pressed), GLFW PRESSED(first frame pressed), or GLFW_REPEAT(pressed but not first frame)
		static uint8_t GetKeyState(uint32_t key);
		static uint8_t GetMouseButtonState(uint32_t button);
		// Get Mouse Position
		static Vec2 GetMousePos();
	private:
		// Stores key and mouse button states
		static std::unordered_map<uint32_t, uint8_t> Keys;
		static std::unordered_map<uint8_t, uint8_t> MouseButtons;
		// Stores mouse position
		static Vec2 MousePos;
		// Stores mouse button state from last frame, used in updating to GLFW_REPEAT
		static std::unordered_map<uint8_t, uint8_t> MouseButtonsLast;
	};
}