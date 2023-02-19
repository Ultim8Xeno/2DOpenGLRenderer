#version 450 core

layout(location = 0) in vec4 inColor;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in flat int texID;

out vec4 outColor;

uniform sampler2D u_Textures[16];

void main()
{
	outColor = texture(u_Textures[int(texID)], texCoords).rgba * inColor;
}