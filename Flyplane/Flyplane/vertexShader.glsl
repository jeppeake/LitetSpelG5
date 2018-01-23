layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex;
uniform mat4 modelMatrix;
uniform mat4 ViewProjMatrix;
varying vec2 Tex;
void main() {
	Tex = tex;
	gl_Position = ViewProjMatrix * modelMatrix * vec4(pos, 1.0);
}