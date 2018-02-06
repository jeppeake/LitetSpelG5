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
uniform vec3 spawn;
uniform vec3 direction;
uniform float dt;
layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;
void main()
{
	uint gid = gl_GlobalInvocationID.x;
	vec3 pos = Positions[gid].xyz;
	vec3 vel = Velocities[gid].xyz;
	vec4 color = Colors[gid];
	float life = Lives[gid];
	Positions[gid] *= 1.0 / 60.0;
}