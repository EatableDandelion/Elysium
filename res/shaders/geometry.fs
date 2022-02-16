#version 330

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gDiffuse;
layout(location = 2) out vec4 gNormal;

uniform sampler2D diffuse;

in vec3 normal;
in vec4 position;
in vec2 texCoord;

void main()
{
	gPosition = vec4(0.5*position.xyz+0.5, 0.0);
	gNormal   = vec4(0.5*normal+0.5, 0.0); 
	gDiffuse  = texture2D(diffuse, texCoord);
}
