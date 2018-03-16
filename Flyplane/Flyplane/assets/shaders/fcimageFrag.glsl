#version 430
uniform sampler2D texSampler;

in vec2 tex;
void main()
{
	vec4 color = texture(texSampler, vec2(tex.x, 1 - tex.y)).rgba;
	gl_FragColor = color;
}