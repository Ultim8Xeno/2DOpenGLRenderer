#pragma once
#include <string>
#include "Vector.h"
#include <stdint.h>

namespace OpenGLRenderer {

	// OpenGL FrameBuffer Object
	class FrameBuffer
	{
	public:
		FrameBuffer(const Vec2& size);
		~FrameBuffer();

		void Bind();

		void Start();
		void End();

		uint32_t GetTextureID() const { return m_TextureID; }

	private:
		uint32_t m_RendererID;
		uint32_t m_TextureID;
	};
}