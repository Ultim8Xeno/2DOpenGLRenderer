#include "Text.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Debug.h"
#include "Renderer.h"
#include <glad/glad.h>
#include <cmath>

namespace OpenGLRenderer {

	// Get substring of value of id
	static std::string GetStr(const std::string& inStr, const std::string& id, char end)
	{
		std::string outStr = "";
		uint32_t offset = inStr.find(id) + id.length();
		outStr = inStr.substr(offset, inStr.find(end, offset) - offset);
		return outStr;
	}

	TextObj::TextObj(const std::string& filepath)
	{
		// Open file and check for errors
		std::ifstream file(filepath);
		ASSERT(file.is_open());
		std::string s;
		// Get 3rd line
		for (int i = 0; i < 3; i++)
			std::getline(file, s);
		ASSERT(!s.empty());
		
		// Get filepath for spritesheet of font
		std::string fileDir = "";
		fileDir = filepath.substr(0, filepath.find_last_of('/') + 1);
		std::string textureFilePath = fileDir + GetStr(s, "file=\"", '"');

		// Create texture from spritesheet
		m_Texture = new Texture(textureFilePath);

		// Get the amount of characters
		std::getline(file, s);
		uint32_t charCount = atoi(GetStr(s, "count=", '\n').c_str());

		// Get all useful information for each character
		for (int i = 0; i < charCount; i++)
		{
			std::getline(file, s);
			uint32_t id = atoi(GetStr(s, "id=", ' ').c_str());
			m_CharacterData[id].dims = Vec2(
				atoi(GetStr(s, "width=", ' ').c_str()),
				atoi(GetStr(s, "height=", ' ').c_str())
			);
			m_CharacterData[id].texCoords = Vec2(
				atoi(GetStr(s, "x=", ' ').c_str()),
				m_Texture->GetSize().y - atoi(GetStr(s, "y=", ' ').c_str()) - m_CharacterData[id].dims.y
			);
			m_CharacterData[id].offset = Vec2(
				atoi(GetStr(s, "xoffset=", ' ').c_str()),
				atoi(GetStr(s, "yoffset=", ' ').c_str())
			);
			m_CharacterData[id].advance = atoi(GetStr(s, "xadvance=", ' ').c_str());

			// Get max height
			m_MaxHeight = std::max(m_CharacterData[id].dims.y, (float)m_MaxHeight);
		}
		
		// Get the amount of kernings
		std::getline(file, s);
		uint32_t kerningsCount = atoi(GetStr(s, "kernings count=", '\n').c_str());

		// Get kernings, where a.kernings[b] is the additional space between ab
		for (int i = 0; i < kerningsCount; i++)
		{
			std::getline(file, s);
			uint32_t first = atoi(GetStr(s, "first=", ' ').c_str());
			uint32_t second = atoi(GetStr(s, "second=", ' ').c_str());
			int amount = atoi(GetStr(s, "amount=", '\n').c_str());
			m_CharacterData[first].kernings[second] = amount;
		}
	}

	TextObj::~TextObj()
	{
		// Clean up memory
		delete m_Texture;
	}

	void TextObj::Write(const std::string& text, const Vec3& pos, float scale, const Vec4& col, float rot)
	{
		// Get dimensions of box and top left
		Vec2 dimensions = GetTextBoxDimensions(text, scale);
		Vec2 topLeft = Vec2(pos.x - dimensions.x / 2, pos.y + dimensions.y / 2);

		// top left of each character
		float xPos = topLeft.x;
		float yPos = topLeft.y;

		// Go through every character and render in correct location while making necessary changes to position of next character
		for (int i = 0; i < text.length(); i++)
		{
			char c = text[i];
			// If new line, adjust y position to next line and x position to beginning of line
			if (c == '\n')
			{
				yPos -= m_MaxHeight * scale;
				xPos = topLeft.x;
				continue;
			}

			// Convert top left of character box to middle of character texture
			Vec3 letterPos = Vec3(
				xPos + m_CharacterData[c].dims.x * scale / 2 + m_CharacterData[c].offset.x * scale
				, yPos - m_CharacterData[c].dims.y * scale / 2 - m_CharacterData[c].offset.y * scale
				, pos.z
			);

			// Render character
			letterPos.RotateAroundCenter(rot, Vec2(pos.x, pos.y));
			Renderer::DrawFromSpriteSheet(letterPos, Vec2(m_CharacterData[c].dims.x * scale, m_CharacterData[c].dims.y * scale),
				m_Texture, m_CharacterData[c].texCoords, m_CharacterData[c].dims, col, rot);

			// Add to x position for next character
			xPos += m_CharacterData[c].advance * scale;
			if (i < text.length() - 1)
			{
				if (m_CharacterData[c].kernings.find(text[i + 1]) != m_CharacterData[c].kernings.end())
				{
					xPos += m_CharacterData[c].kernings[text[i + 1]] * scale;
				}
			}
		}
	}

	Vec2 TextObj::GetTextBoxDimensions(const std::string& text, float scale)
	{
		// Set default text size to (0, size of one line)
		Vec2 text_size = Vec2(0.0f, m_MaxHeight * scale);
		// Stores current line length
		float lineLen = 0.0f;
		for (int i = 0; i < text.length(); i++)
		{
			// If new line character, increase y size by one line
			if (text[i] == '\n')
			{
				if (i)
				{
					// Only count dimension of previous character instead of advance
					lineLen -= m_CharacterData[text[i - 1]].advance * scale;
					lineLen += m_CharacterData[text[i - 1]].dims.x * scale;
				}
				text_size.y += m_MaxHeight * scale;

				// x size is the length of biggest line
				text_size.x = std::max(text_size.x, lineLen);

				lineLen = 0.0f;
			}

			// Add character advance to line length
			lineLen += m_CharacterData[text[i]].advance * scale;
		}
		// Only count dimension of previous character instead of advance
		lineLen -= m_CharacterData[text[text.length() - 1]].advance * scale;
		lineLen += m_CharacterData[text[text.length() - 1]].dims.x * scale;

		// x size is the length of biggest line
		text_size.x = std::max(text_size.x, lineLen);
		
		return Vec2(text_size.x, text_size.y);
	}
}