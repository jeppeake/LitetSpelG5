#version 420
uniform sampler2D texSampler;
varying vec2 Tex;

void main() {
	vec4 color = texture(texSampler, vec2(Tex.x, 1 - Tex.y));
	gl_FragColor = color;
}