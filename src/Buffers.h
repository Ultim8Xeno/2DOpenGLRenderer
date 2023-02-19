#pragma once
#include <string>
#include <glad/glad.h>
#include <stdint.h>

namespace OpenGLRenderer {

	// OpenGL Vertex Buffer Object
	class VertexBuffer
	{
	public:
		// Constructor, takes in vertex data and size of vertex data
		VertexBuffer(void* data, uint32_t size);
		// Destructor
		~VertexBuffer();
		// Bind Vertex Buffer
		void Bind() { glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); }
	private:
		// OpenGL id
		uint32_t m_RendererID = 0;
	};
	
	// OpenGL Index Buffer Object
	class IndexBuffer
	{
	public:
		// Constructor, takes in indices data and size of indices data
		IndexBuffer(uint32_t* data, uint32_t size);
		// Destructor
		~IndexBuffer();
		// Bind Index Buffer
		void Bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); }
	private:
		// OpenGL id
		uint32_t m_RendererID = 0;
	};
}