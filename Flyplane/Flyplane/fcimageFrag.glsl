#version 430
uniform sampler2D texSampler;

in vec2 tex;
void main()
{
	vec3 color = texture(texSampler, vec2(tex.x, 1 - tex.y)).rgb;
	gl_FragColor = vec4(color, 1.0f);
}