#version 420
//uniform sampler2D texSampler;
//varying vec2 Tex;
uniform vec3 color;

void main() {
	//vec4 color = texture(texSampler, vec2(Tex.x, 1 - Tex.y));
	gl_FragColor = vec4(color, 0.4);
}