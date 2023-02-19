#include "Renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Buffers.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include <cmath>
#include <cstring>
#include <iostream>
#include "Debug.h"
#include "Input.h"
#include "Text.h"

namespace OpenGLRenderer {

	// Data for renderer
	struct RendererData
	{
		// Makes sure too much memory isn't used
		const uint32_t MAX_QUADS = 250;
		const uint32_t MAX_INDICES = MAX_QUADS * 6;
		const uint32_t MAX_VERTICES = MAX_QUADS * 4;
		const uint32_t MAX_TEXTURES = 16;

		// Index for vertex data
		uint32_t currentQuad = 0;
		// Index for texture ids
		uint32_t currentTex = 0;

		// OpenGL Objects
		GLFWwindow* window = nullptr;
		VertexBuffer* vb = nullptr;
		IndexBuffer* ib = nullptr;
		VertexArray* va = nullptr;
		Shader* shader = nullptr;

		// Data for vertices and indices
		struct VertexData
		{
			float pos[3] = { 0.0f, 0.0f, 0.0f };
			float col[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			float texCoords[2] = { 0.0f, 0.0f };
			int texID = 0;
		};
		VertexData* vertexData = nullptr;
		uint32_t* indexData = nullptr;

		// Texture ids
		uint32_t* textures = nullptr;
		Texture* whiteTex = nullptr;

		// Default text object
		TextObj* textObj = nullptr;

		// Time between frames
		float dt = 0.0f;
		// Used to calculate dt
		float lastFrameTime = 0.0f;
	};

	static RendererData data;

	void Renderer::Setup(GLFWwindow* window)
	{
		// Set variables
		data.currentQuad = 0;
		data.currentTex = 0;
		data.window = window;
		
		// Allocate memory
		data.vertexData = new RendererData::VertexData[data.MAX_VERTICES]();
		memset(data.vertexData, 0, data.MAX_VERTICES * sizeof(RendererData::VertexData));
		data.indexData = new uint32_t[data.MAX_INDICES];

		// Create Shader
		data.shader = new Shader("Res/Shaders/vertex.shader", "Res/Shaders/fragment.shader");
		
		// Create Vertex Array
		data.va = new VertexArray();

		// Set indices data
		// indexData = { 0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, ... }
		for (uint32_t i = 0; i < data.MAX_QUADS; i++)
		{
			data.indexData[i * 6 + 0] = i * 4 + 0;
			data.indexData[i * 6 + 1] = i * 4 + 1;
			data.indexData[i * 6 + 2] = i * 4 + 2;
			data.indexData[i * 6 + 3] = i * 4 + 2;
			data.indexData[i * 6 + 4] = i * 4 + 3;
			data.indexData[i * 6 + 5] = i * 4 + 0;
		}

		// Create Index Buffer object with indices data
		data.ib = new IndexBuffer(data.indexData, sizeof(uint32_t) * data.MAX_INDICES);

		// Create Vertex Buffer object with vertex data
		data.vb = new VertexBuffer(data.vertexData, sizeof(RendererData::VertexData)*data.MAX_VERTICES);

		// Set Vertex Array Attributes
		data.va->SetAttributes({
			VertexAttribute(GL_FLOAT, 3),	// Position (x, y, z)
			VertexAttribute(GL_FLOAT, 4),	// Colour (r, g, b, a)
			VertexAttribute(GL_FLOAT, 2),	// Texture Coordinates (x, y)
			VertexAttribute(GL_INT, 1)		// Texture id
			});

		// Set uniform to (0, 1, 2, ..., MAX_TEXTURES-1)
		int* samplers = new int[data.MAX_TEXTURES];
		for (int i = 0; i < data.MAX_TEXTURES; i++)
			samplers[i] = i;

		data.shader->SetUniform(samplers, "u_Textures", GL_INT, data.MAX_TEXTURES);
		
		delete[] samplers;

		// set variable, holds tex ids of textures to render
		data.textures = new uint32_t[data.MAX_TEXTURES];
		memset(data.textures, 0, data.MAX_TEXTURES * sizeof(uint32_t));

		// Create default texture (white pixel), used in Renderer::DrawQuad(...);
		uint32_t whiteTexData = 0xffffffff;
		data.whiteTex = new Texture(&whiteTexData, 1, 1);

		// Create text object with default font
		data.textObj = new TextObj("Res/Fonts/RobotoCondensed-Regular.fnt");
		
		// Set background colour to light gray
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

		// Enable z position
		glEnable(GL_DEPTH_TEST);
		
		// Enable Blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Set lastFrameTime
		data.lastFrameTime = glfwGetTime();
	}
	
	void Renderer::Shutdown()
	{
		// Clean up memory
		delete data.va;
		delete data.ib;
		delete data.vb;
		delete data.shader;
		delete data.whiteTex;
		delete data.textObj;
		delete[] data.indexData;
		delete[] data.vertexData;
		delete[] data.textures;
	}

	void Renderer::ClearColour(const Vec4& colour)
	{
		// Set background colour to colour
		glClearColor(colour.x, colour.y, colour.z, colour.w);
	}

	void Renderer::BeginDraw()
	{
		// Clear colour and depth
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Calculate delta time (time between frames)
		data.dt = glfwGetTime() - data.lastFrameTime;
		data.lastFrameTime = glfwGetTime();
	}

	void Renderer::Flush()
	{
		// Bind every texture in data.textures
		for (int i = 0; i < data.currentTex; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, data.textures[i]);
		}

		// Set vertex buffer data to data.vertexData
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(RendererData::VertexData) * 4 * data.currentQuad, data.vertexData);
		// Draw all triangles
		glDrawElements(GL_TRIANGLES, data.currentQuad * 6, GL_UNSIGNED_INT, 0);

		// Reset variables
		data.currentQuad = 0;
		data.currentTex = 0;
	}

	void Renderer::SetBounds(const Vec2& bounds)
	{
		// Converts -1 to 1 into 0 to bounds.x and 0 to bounds.y
		float dims[2] = { bounds.x, bounds.y };
		data.shader->SetUniform(dims, "u_MonitorBounds", GL_FLOAT_VEC2, 1);
	}

	float Renderer::GetDT()
	{
		// Get time between frames
		return data.dt;
	}

	void Renderer::EndDraw()
	{
		// Render
		Flush();
		
		// Swap buffers
		glfwSwapBuffers(data.window);

		// Get input
		glfwPollEvents();
		Input::Update();
	}

	void Renderer::DrawQuad(const Vec3& pos, const Vec2& dim, const Vec4& col, float rot)
	{
		// Draw a texture with data.whiteTex
		DrawTexture(pos, dim, data.whiteTex, col, rot);
	}

	void Renderer::DrawTexture(const Vec3& pos, const Vec2& dim, Texture* tex, const Vec4& col, float rot)
	{
		// Draw texture as spritesheet with one sprite
		DrawFromSpriteSheet(pos, dim, tex, Vec2(0.0f, 0.0f), Vec2(tex->GetSize().x, tex->GetSize().y), col, rot);
	}

	void Renderer::DrawTexture(const Vec3& pos, const Vec2& dim, uint32_t texId, const Vec2& textureSize, const Vec4& col, float rot)
	{
		// Draw texture as spritesheet with one sprite
		DrawFromSpriteSheet(pos, dim, texId, textureSize, Vec2(0.0f, 0.0f), textureSize, col, rot);
	}

	void Renderer::DrawFromSpriteSheet(const Vec3& pos, const Vec2& dim, Texture* tex, const Vec2& startingTexPos,
		const Vec2& spriteSize, const Vec4& col, float rot)
	{
		// Draw from sprite sheet with texture id
		DrawFromSpriteSheet(pos, dim, tex->GetID(), tex->GetSize(), startingTexPos, spriteSize, col, rot);
	}

	void Renderer::DrawFromSpriteSheet(const Vec3& pos, const Vec2& dim, uint32_t tex, const Vec2& texSize,
		const Vec2& startingTexPos, const Vec2& spriteSize, const Vec4& col, float rot)
	{
		// Tries to find texture id in already rendered textures, if found set texID to index
		int texID = 0;
		for (int i = 0; i < data.currentTex; i++)
		{
			if (tex == data.textures[i])
				texID = i;
		}
		// Otherwise, put it at the end of the list
		if (!texID)
		{
			data.textures[data.currentTex] = tex;
			texID = data.currentTex;
			data.currentTex++;
		}

		// top left, top right, bottom right, bottom left
		int8_t signs[8] = { -1, 1, 1, 1, 1, -1, -1, -1 };
		// diagonal length
		double r = 0.0;
		if (rot)
			r = sqrt(dim.x * dim.x + dim.y * dim.y) / 2;

		for (int i = 0; i < 4; i++)
		{
			// Set positions of vertices
			if (rot)
			{
				// If rotation, set them using some trigonometry I'm too lazy to explain
				float angle = std::atan2(dim.y / 2 * signs[2 * i + 1], dim.x / 2 * signs[2 * i]);
				data.vertexData[data.currentQuad * 4 + i].pos[0] = pos.x + r * std::cos(angle - rot * 0.0174532925);
				data.vertexData[data.currentQuad * 4 + i].pos[1] = pos.y + r * std::sin(angle - rot * 0.0174532925);
			}
			else
			{
				// Set them normally
				data.vertexData[data.currentQuad * 4 + i].pos[0] = pos.x + signs[2 * i] * dim.x / 2;
				data.vertexData[data.currentQuad * 4 + i].pos[1] = pos.y + signs[2 * i + 1] * dim.y / 2;
			}

			// Set all vertex data
			data.vertexData[data.currentQuad * 4 + i].pos[2] = pos.z/-100000.0f;
			data.vertexData[data.currentQuad * 4 + i].col[0] = col.x;
			data.vertexData[data.currentQuad * 4 + i].col[1] = col.y;
			data.vertexData[data.currentQuad * 4 + i].col[2] = col.z;
			data.vertexData[data.currentQuad * 4 + i].col[3] = col.w;
			data.vertexData[data.currentQuad * 4 + i].texCoords[0] = (startingTexPos.x + (signs[2 * i] == 1 ? 1 : 0) * spriteSize.x) / texSize.x;
			data.vertexData[data.currentQuad * 4 + i].texCoords[1] = (startingTexPos.y + (signs[2 * i + 1] == 1 ? 1 : 0) * spriteSize.y) / texSize.y;
			data.vertexData[data.currentQuad * 4 + i].texID = texID;
		}
		data.currentQuad++;

		// If max amount of quads in one batch or max amount of textures in one batch, render it to reset
		if (data.currentQuad >= data.MAX_QUADS || data.currentTex >= data.MAX_TEXTURES)
			Flush();
	}

	void Renderer::Write(const std::string& text, const Vec3& pos, float size, const Vec4& col, float rot)
	{
		// Write with the default font
		data.textObj->Write(text, pos, size, col, rot);
	}

	void Renderer::DrawLine(const Vec2& start, const Vec2& end, const Vec4& col, float width, float zIndex)
	{
		// Center is average of start and end
		Vec3 center = { (end.x + start.x) / 2, (start.y + end.y) / 2, zIndex };
		// Dimensions is (width, length between start and end)
		Vec2 dim = { width, std::sqrt((end.x - start.x) * (end.x - start.x) + (end.y - start.y) * (end.y - start.y)) };
		// Get rotation in rad, convert to deg, subtract 90 because reasons
		float rot = std::atan2(end.y - start.y, end.x - start.x) * -57.2957795f + 90.0f;

		// Just draw it as a quad
		DrawQuad(center, dim, col, rot);
	}
}