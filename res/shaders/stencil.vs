#version 330

layout(location = 0) in vec3 position0;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(position0, 1.0);
}
