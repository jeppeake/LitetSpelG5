#shader vertex

layout(location = 0) in vec4 position;

void main()
{
	gl_Position = position;
}

#shader fragment

void main()
{
	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}