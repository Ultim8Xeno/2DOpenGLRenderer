#pragma once
#include <string>
#include <glad/glad.h>
#include <stdint.h>

namespace OpenGLRenderer {

	// OpenGL Shader Class
	class Shader
	{
	public:
		// Constructor, takes in filepath of vertex and fragment shaders
		Shader(const std::string& vertShaderfilepath, const std::string& fragShaderfilepath);
		// Destructor
		~Shader();
		// Bind shader
		void Bind() { glUseProgram(m_RendererID); }
		// Set Uniform, takes in data, location, type, and count
		void SetUniform(void* data, const std::string& location, uint32_t type, uint32_t count);
	private:
		// Read file into string
		std::string ReadFile(const std::string& filepath);
		
		// OpenGL id
		uint32_t m_RendererID = 0;
	};
}