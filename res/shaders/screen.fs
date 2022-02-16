#version 330

out vec4 color;

in vec2 texCoord;

uniform sampler2D gDiffuse;

uniform float ambient;

void main()
{
	color = ambient*texture2D(gDiffuse, texCoord);
}
