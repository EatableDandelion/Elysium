#version 330

out vec4 color;

in vec2 texCoord;

uniform sampler2D gDiffuse;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 lightAttenuation;
uniform vec2 screenSize;

#include "lighting.glsl"

void main()
{
	vec2 uv = gl_FragCoord.xy/screenSize;
	color = texture2D(gDiffuse, uv)
		  * calcPointLight(lightPosition, lightColor, 
				  		   lightAttenuation, uv);
}
