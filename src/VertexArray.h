#pragma once
#include <string>
#include <vector>
#include <glad/glad.h>
#include <stdint.h>

namespace OpenGLRenderer {

	// Vertex Attribute
	struct VertexAttribute
	{
		// Constructor, takes in GL_ENUM and count
		VertexAttribute(uint32_t t, uint32_t c) { type = t; count = c; }
		uint32_t type;
		uint32_t count;
	};

	// OpenGL Vertex Array Object
	class VertexArray
	{
	public:
		// Constructor
		VertexArray();
		// Destructor
		~VertexArray();
		// Set attributes, takes in vector of VertexAttributes
		void SetAttributes(const std::vector<VertexAttribute>& attributes);
		// Bind
		void Bind() { glBindVertexArray(m_RendererID); }
	private:
		// OpenGL id
		uint32_t m_RendererID = 0;
	};
}