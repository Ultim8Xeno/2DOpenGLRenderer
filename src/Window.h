#pragma once
#include "Vector.h"
#include <string>
#include <stdint.h>

// WINDOW_HINTS
#define WINDOW_MAXIMIZED_HINT 1
#define WINDOW_RESIZE_HINT 2
#define WINDOW_SCALE_TO_MONITOR_HINT 4

struct GLFWwindow;

namespace OpenGLRenderer {
	
	// Initialize Window to create window
	class Window
	{
	public:
		// Constructor, Takes in name of window, dimensions, and any window hints (line 8-10)
		Window(const std::string& name, const Vec2& dim, uint8_t windowHints = 0);
		// Destructor
		~Window();

		// Window Closing Functions
		bool ShouldWindowClose() const;
		void CloseWindow();

		// Get GLFW Window
		GLFWwindow* GetWindow() { return pWindow; }
		// Get Window Dimensions
		Vec2 GetWindowSize() { return dimensions; }
	private:
		// Pointer to GLFW window
		GLFWwindow* pWindow;
		// Dimensions of window
		Vec2 dimensions;
	};
}