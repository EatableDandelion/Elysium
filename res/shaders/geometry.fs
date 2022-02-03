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
	gPosition = position;
	gNormal   = vec4(normal, 0.0); 
	gDiffuse  = texture2D(diffuse, texCoord);
}
