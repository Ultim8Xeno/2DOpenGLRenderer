#pragma once
#include "Window.h"
#include "Vector.h"
#include "Input.h"
#include "Debug.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "Text.h"

namespace OpenGLRenderer {

	// Render Objects onto screen
	class Renderer
	{
	public:
		// Setup Renderer
		static void Setup(GLFWwindow* window);
		// Shutdown Renderer
		static void Shutdown();

		// Set Background colour
		static void ClearColour(const Vec4& colour);

		// Call before drawing every frame
		static void BeginDraw();
		// Call at the end to render every frame
		static void EndDraw();

		// Draw functions
		static void DrawQuad(const Vec3& pos, const Vec2& dim, const Vec4& col, float rot = 0.0f);
		static void DrawTexture(const Vec3& pos, const Vec2& dim, Texture* tex, const Vec4& col = Vec4(1.0f, 1.0f, 1.0f, 1.0f),
			float rot = 0.0f);
		static void DrawTexture(const Vec3& pos, const Vec2& dim, uint32_t texId, const Vec2& textureSize, const Vec4& col = Vec4(1.0f, 1.0f, 1.0f, 1.0f),
			float rot = 0.0f);
		static void DrawFromSpriteSheet(const Vec3& pos, const Vec2& dim, Texture* tex, const Vec2& startingTexPos,
			const Vec2& spriteSize, const Vec4& col = Vec4(1.0f, 1.0f, 1.0f, 1.0f), float rot = 0.0f);
		static void DrawFromSpriteSheet(const Vec3& pos, const Vec2& dim, uint32_t texId, const Vec2& textureSize,
			const Vec2& startingTexPos, const Vec2& spriteSize, const Vec4& col = Vec4(1.0f, 1.0f, 1.0f, 1.0f),
			float rot = 0.0f);
		static void DrawLine(const Vec2& start, const Vec2& end, const Vec4& col, float width, float zIndex = 0.0f);

		// Render text to screen
		static void Write(const std::string& text, const Vec3& pos, float scale = 1.0f, const Vec4& col = Vec4(1.0f, 1.0f, 1.0f, 1.0f), float rot = 0.0f);

		// Set Window Bounds
		static void SetBounds(const Vec2& bounds);

		// Get time since last frame (delta time)
		static float GetDT();

		// Actually Renders everything
		static void Flush();
	};
}