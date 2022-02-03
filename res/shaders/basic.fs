#version 330

out vec4 color;

uniform sampler2D diffuse;

varying vec3 normal0;
varying vec2 texCoord0;

void main()
{
	color = texture2D(diffuse, texCoord0);
	color = color * (0.8*clamp(dot(normal0, vec3(0.5,1,0)), 0, 1)+0.2);
}
