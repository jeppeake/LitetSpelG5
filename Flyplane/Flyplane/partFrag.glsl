#version 430
layout(location = 0) out vec4 color;
in vec4 fragCol;
void main()
{
	color = fragCol;
}