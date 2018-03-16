#version 430
layout (location = 0) in vec2 position;

out vec2 tex;
void main()
{
   tex = position;
   gl_Position = vec4(position*2-1, 0.0f, 1.0f);
}