#include "Texture.h"
#include "Debug.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "Vendor/stb_image.h"

namespace OpenGLRenderer {

	Texture::Texture(const std::string& filepath)
	{
		// Create texture, set parameters
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Load with stbi_image
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
		// Check data exists
		ASSERT(data);
		// Set texture size
		m_TexSize = Vec2(width, height);
		
		// Get format and internal format
		GLenum format = 0;
		GLenum intFormat = 0;
		switch (nrChannels)
		{
			case 3:
				format = GL_RGB;
				intFormat = GL_RGB8;
				break;
			case 4:
				format = GL_RGBA;
				intFormat = GL_RGBA8;
				break;
			default:
				// Unsupported Format
				ASSERT(false);
				break;
		}

		// Generate texture from data
		glTexImage2D(GL_TEXTURE_2D, 0, intFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Clean up memory
		stbi_image_free(data);
	}

	Texture::Texture(void* data, int width, int height)
	{
		// Create texture and set parameters
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Set texture size
		m_TexSize = Vec2((float)width, (float)height);

		// Generate texture from data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	Texture::~Texture()
	{
		// Clean up memory
		glDeleteTextures(1, &m_RendererID);
	}
}