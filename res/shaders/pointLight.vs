#version 330

layout(location = 0) in vec3 position0;
layout(location = 2) in vec2 texCoord0;

uniform mat4 MVP;

out vec2 texCoord;

void main()
{
	gl_Position = MVP*vec4(position0, 1.0);
	texCoord = texCoord0;
}
