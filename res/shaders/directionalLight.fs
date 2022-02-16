#version 330

out vec4 color;

in vec2 texCoord;

uniform sampler2D gDiffuse;
uniform vec3 lightColor;
uniform vec3 lightDirection;

#include "lighting.glsl"

void main()
{
	color = texture2D(gDiffuse, texCoord)
		  * calcDirectionalLight(lightColor, lightDirection, texCoord);
}
