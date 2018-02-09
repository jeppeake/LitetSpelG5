#version 430 core

in vec4 passCol[];
out vec4 fragCol;
layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;  

uniform mat4 view;
uniform mat4 projection;

uniform vec3 cPos;
uniform vec3 cUp;

uniform float particleSize;
void main()
{
	vec3 pos = gl_in[0].gl_Position.xyz;
	vec3 look = cPos - pos;
	look = normalize(look);
	vec3 right = -cross(cUp, look);
	vec3 up = -cross(look, right);
	
	fragCol = passCol[0];
	vec3 vert1 = pos - (right + up) * particleSize;
	gl_Position = projection * view * vec4(vert1, 1.0);
	EmitVertex();

	fragCol = passCol[0];
	vec3 vert2 = pos - (right - up) * particleSize;
	gl_Position = projection * view * vec4(vert2, 1.0);
	EmitVertex();

	fragCol = passCol[0];
	vec3 vert3 = pos + (right - up) * particleSize;
	gl_Position = projection * view * vec4(vert3, 1.0);
	EmitVertex();

	fragCol = passCol[0];
	vec3 vert4 = pos + (right + up) * particleSize;
	gl_Position = projection * view * vec4(vert4, 1.0);
	EmitVertex();

	EndPrimitive();  
}