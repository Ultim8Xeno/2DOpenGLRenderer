#pragma once
#include <string>
#include "Vector.h"
#include <stdint.h>

namespace OpenGLRenderer
{
	// OpenGL Texture object
	class Texture
	{
	public:
		// Constructors
		// Create texture from filepath
		Texture(const std::string& filepath);
		// Creates texture from data
		Texture(void* data, int width, int height);

		// Destructor
		~Texture();

		// Get texture id
		uint32_t GetID() const { return m_RendererID; }
		// Get size of texture
		const Vec2& GetSize() const { return m_TexSize; }
	private:
		// OpenGL id
		uint32_t m_RendererID = 0;
		// Size of Texture
		Vec2 m_TexSize = Vec2(0.0f, 0.0f);
	};
}