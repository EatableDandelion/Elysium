#version 330

out vec4 color;

in vec2 texCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gDiffuse;

void main()
{
	color = texture2D(gDiffuse, texCoord);
}
