#include "VertexArray.h"
#include <glad/glad.h>
#include "Debug.h"

namespace OpenGLRenderer {
	
	// Get size from type
	static uint32_t TypeToSize(GLenum type)
	{
		switch (type)
		{
		case GL_FLOAT: return sizeof(float);
		case GL_INT: return sizeof(int);
		case GL_BOOL: return sizeof(bool);
		default:
			// type not supported
			ASSERT(false);
			return 0;
			break;
		}
	}

	VertexArray::VertexArray()
	{
		// Create vertex array object
		glGenVertexArrays(1, &m_RendererID);
		glBindVertexArray(m_RendererID);
	}

	VertexArray::~VertexArray()
	{
		// Delete vertex array object
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void VertexArray::SetAttributes(const std::vector<VertexAttribute>& attributes)
	{
		// Offset, bytes offset from start of vertex to start of attribute
		uint32_t offset = 0;
		// Stride, size of vertex in bytes
		uint32_t stride = 0;

		// Calculate stride
		for (int i = 0; i < attributes.size(); i++)
			stride += attributes[i].count * TypeToSize(attributes[i].type);

		// Set each attribute based on type
		for (int i = 0; i < attributes.size(); i++)
		{
			switch (attributes[i].type)
			{
			case GL_FLOAT:
				glVertexAttribPointer(i, attributes[i].count, attributes[i].type, GL_FALSE, stride, (void*)offset);
				break;
			case GL_INT:
				glVertexAttribIPointer(i, attributes[i].count, attributes[i].type, stride, (void*)offset);
				break;
			default:
				// type not supported
				ASSERT(false);
				break;
			}
			glEnableVertexAttribArray(i);

			// Add to offset
			offset += attributes[i].count * TypeToSize(attributes[i].type);
		}
	}
}