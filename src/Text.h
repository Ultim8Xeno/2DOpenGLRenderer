#pragma once
#include <string>
#include "Vector.h"
#include <unordered_map>
#include "Texture.h"
#include <stdint.h>

namespace OpenGLRenderer {

	// Text Object
	class TextObj
	{
	public:
		// Constructor, takes in .fnt filepath
		TextObj(const std::string& filepath);
		// Destructor
		~TextObj();

		// Write, takes in text, position, scale, tint, rotation
		void Write(const std::string& text, const Vec3& pos, float scale = 1.0f, const Vec4& col = Vec4(1.0f, 1.0f, 1.0f, 1.0f), float rot = 0.0f);

		// Get Dimensions for text box ; (width, height) => (x, y)
		Vec2 GetTextBoxDimensions(const std::string& text, float scale);
	private:
		// Character data
		struct Character
		{
			// Texture Coordinates of bottom left in texture
			Vec2 texCoords;
			// Texture dimensions of sprite in texture
			Vec2 dims;
			// Offset of character after previous character
			Vec2 offset;
			// Advance after rendering current character
			uint32_t advance = 0;
			// Kernings for specific character pairs
			std::unordered_map<char, int> kernings;
		};
		// All character data
		std::unordered_map<char, Character> m_CharacterData;
		// Texture of characters
		Texture* m_Texture = nullptr;
		uint32_t m_MaxHeight = 0;
	};
}