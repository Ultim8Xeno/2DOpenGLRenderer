#version 450 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in int texID;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec2 outTexCoords;
layout(location = 2) out flat int outTexID;
uniform vec2 u_MonitorBounds;

void main()
{
	gl_Position = vec4(2*pos.x/u_MonitorBounds.x-1, 2*pos.y/u_MonitorBounds.y-1, pos.z, 1.0);
	outColor = color;
	outTexCoords = texCoords;
	outTexID = texID;
}