#version 430
layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

out vec4 fragCol;

uniform mat4 view;
uniform mat4 projection;

uniform vec3 cPos;
uniform vec3 cUp;

void main()
{
   fragCol = color;
   gl_Position = projection * view * vec4(position.xyz, 1.0f);
}