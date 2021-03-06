#version 430

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

out vec4 passCol;
out float size;


void main()
{
   passCol = color;
   size = position.w;
   gl_Position = vec4(position.xyz, 1.0f);
}