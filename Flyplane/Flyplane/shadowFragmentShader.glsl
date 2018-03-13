#version 420


uniform sampler2D texSampler;

in vec2 Tex;

void main() {
	float alpha = texture(texSampler, vec2(Tex.x, 1 - Tex.y)).a;	
	if(alpha < 150.0/255.0) {
		discard;
	}
}