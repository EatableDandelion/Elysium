#version 120

attribute vec3 position;
attribute vec3 normal;
attribute vec2 texCoord;

varying vec3 normal0;
varying vec2 texCoord0;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(position, 1.0);
	texCoord0 = texCoord;
	normal0 = (MVP * vec4(normal, 0.0)).xyz;
	normal0 = normalize(normal0);
}
