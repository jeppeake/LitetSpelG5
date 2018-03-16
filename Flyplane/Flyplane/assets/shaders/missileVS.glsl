#version 420
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex;
uniform mat4 ViewProjMatrix;
uniform mat4 modelMatrix;

varying vec3 Normal;
varying vec2 Tex;

void main() {
	Tex = tex;
	Normal = transpose(inverse(mat3(modelMatrix)))*normal;
	gl_Position = ViewProjMatrix * modelMatrix * vec4(pos, 1.0);
}