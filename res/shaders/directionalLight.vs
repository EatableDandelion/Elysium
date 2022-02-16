#version 330

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texCoord0;

out vec2 texCoord;

void main()
{
	gl_Position = vec4(position.x, position.y, 0, 1.0);
	texCoord = texCoord0;
}
