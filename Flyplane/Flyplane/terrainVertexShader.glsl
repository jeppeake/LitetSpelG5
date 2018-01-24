layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex;
uniform mat4 modelMatrix;
uniform mat4 ViewProjMatrix;
varying vec3 Pos;
varying vec3 Normal;
varying vec2 Tex;

void main() {
	Pos = (modelMatrix * vec4(pos, 1.0)).xyz;
	Tex = tex;
	Normal = vec3(modelMatrix * vec4(normal, 0));
	gl_Position = ViewProjMatrix * modelMatrix * vec4(pos, 1.0);
}