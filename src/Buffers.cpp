#include "Buffers.h"
#include <glad/glad.h>
#include <iostream>

namespace OpenGLRenderer {

	VertexBuffer::VertexBuffer(void* data, uint32_t size)
	{
		// Create Vertex Buffer and set data
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		// Delete Vertex Buffer
		glDeleteBuffers(1, &m_RendererID);
	}

	IndexBuffer::IndexBuffer(uint32_t* data, uint32_t size)
	{
		// Create Index Buffer and set data
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		// Delete Index Buffer
		glDeleteBuffers(1, &m_RendererID);
	}
}