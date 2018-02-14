#version 430 core
layout(std430, binding=7) buffer Pos
{
	vec4 Positions[];
};
layout(std430, binding=8) buffer Vel
{
	vec4 Velocities[];
};
layout(std430, binding=9) buffer Life
{
	float Lives[];
};
layout(std430, binding=10) buffer Col
{
	vec4 Colors[];
};

layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;

void main()
{
	uint gid = gl_GlobalInvocationID.x;
	Positions[gid] = vec4(0);
	Velocities[gid] = vec4(0);
	Colors[gid] = vec4(vec3(0.7), 1);
	Lives[gid] = 0;
}