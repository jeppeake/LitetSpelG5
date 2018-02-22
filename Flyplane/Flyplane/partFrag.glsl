#version 430
layout(location = 0) out vec4 color;
in vec4 fragCol;
in vec2 texCoords;
uniform sampler2D sampler;
void main()
{
	color = fragCol;
	//color = texture2D(sampler, texCoords);
}