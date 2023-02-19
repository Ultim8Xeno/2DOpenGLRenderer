#include "Window.h"
#include "Debug.h"
#include "Input.h"
#include <iostream>
#include "Renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace OpenGLRenderer {

	Window::Window(const std::string& name, const Vec2& dim, uint8_t windowHints)
		: dimensions(dim)
	{
		// Initialize GLFW and handle errors
		ASSERT(glfwInit());

		// TEMPORARY (hopefully)
		windowHints &= UINT8_MAX ^ WINDOW_RESIZE_HINT;

		// Set window hints
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, windowHints & WINDOW_RESIZE_HINT);
		glfwWindowHint(GLFW_MAXIMIZED, windowHints & WINDOW_MAXIMIZED_HINT);
		glfwWindowHint(GLFW_SCALE_TO_MONITOR, windowHints & WINDOW_SCALE_TO_MONITOR_HINT);

		// Create window and handle errors
		pWindow = glfwCreateWindow((int)dimensions.x, (int)dimensions.y, name.c_str(), NULL, NULL);
		ASSERT(pWindow);
		glfwMakeContextCurrent(pWindow);

		// Get window size
		int w = 0, h = 0;
		glfwGetWindowSize(pWindow, &w, &h);
		dimensions = Vec2(w, h);

		// Set VSync to On
		glfwSwapInterval(1);

		// Set up Input
		Input::Setup(pWindow);

		// Load glad and handle errors
		ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));

		// Setup renderer
		Renderer::Setup(pWindow);

		// Set renderer bounds
		Renderer::SetBounds(dimensions);
	}

	Window::~Window()
	{
		// Cleanup
		Renderer::Shutdown();
		glfwDestroyWindow(pWindow);
		glfwTerminate();
	}

	bool Window::ShouldWindowClose() const
	{
		return glfwWindowShouldClose(pWindow);
	}

	void Window::CloseWindow()
	{
		glfwSetWindowShouldClose(pWindow, true);
	}
}