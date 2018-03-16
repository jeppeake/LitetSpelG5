#version 420
layout(location = 0) in float time;

uniform float size;

void main() {
	int index = gl_VertexID;

	vec2 pos;
	pos.x = 1-float(index)/size;
	pos.y = time;

	vec2 scale = vec2(1, 100);

	pos = scale*pos;
	pos.x -=0.5;
	pos.x *=2;
	pos.y -= 1;

	gl_Position = vec4(pos, 0.0, 1.0);
}