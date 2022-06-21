#version 330

layout(location = 0) in vec3 position0;
layout(location = 1) in vec3 normal0;
layout(location = 2) in vec2 texCoord0;

out vec3 normal;
out vec4 position;
out vec2 texCoord;

uniform mat4 MVP;

void main()
{
	position = (MVP * vec4(position0, 1.0));
	gl_Position = position;
	texCoord = texCoord0;
	normal = normalize((MVP * vec4(normal0, 0.0)).xyz);
}
