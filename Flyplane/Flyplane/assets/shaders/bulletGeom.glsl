#version 430 core

layout (points) in;
layout (line_strip) out;
layout (max_vertices = 2) out;

in vec3 Pos[];
in vec4 Orientation[];

uniform mat4 ViewProjMatrix;

// https://www.opengl.org/discussion_boards/showthread.php/160134-Quaternion-functions-for-GLSL
vec3 qtransform( vec4 q, vec3 v ){ 
	vec3 quatv = q.xyz;
	vec3 uv = cross(quatv, v);
	vec3 uuv = cross(quatv, uv);
	return v + ((uv *q.w) + uuv)*2.0;
}

void main() {
	
	vec3 forward = 0.8*qtransform(Orientation[0], vec3(0,0,1));

	gl_Position = ViewProjMatrix * vec4(Pos[0] + forward, 1);
	EmitVertex();

	gl_Position = ViewProjMatrix * vec4(Pos[0] - forward, 1);
	EmitVertex();

	EndPrimitive();  
}