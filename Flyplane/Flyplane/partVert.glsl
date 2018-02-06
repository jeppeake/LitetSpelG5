#version 430
in vec4 vertexPos;
in vec4 inColor;
uniform mat4 view;
uniform mat4 projection;
out vec4 vertColor;
void main()
{
	gl_Position = projection * view * vertexPos;
	gl_PointSize = 5.0;
	vertColor = inColor;
}