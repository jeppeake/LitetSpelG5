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
uniform float dt;
uniform float life;
uniform vec3 spawn;
uniform float explosionRadius;
layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;
float rand(float n) { 
	return 2.0*fract(sin(n * 12.9898) * 43758.5453)-1.0;
}
void main()
{
	uint gid = gl_GlobalInvocationID.x;
	if(Positions[gid].xyz == vec3(0))
	{
		Positions[gid].xyz = spawn + 10 * normalize(vec3(rand(gid * 2), rand(gid * 3), rand(gid * 5)));
		Lives[gid] = 0;
		Colors[gid].xyz = vec3(1.0,1.0,0.0);

		vec3 vel;
		vel.x = rand(gid * 17);
		vel.y = rand(gid * 7);
		vel.z = rand(gid * 11);
		vel = normalize(vel);


		Velocities[gid].xyz = 3.0 * explosionRadius * pow(rand(gid * 13)*0.5+0.5, 0.3)* vel;
	}
	else
	{
		Velocities[gid].xyz -= Velocities[gid].xyz * (1 - pow(0.01, dt));
		Positions[gid].xyz += Velocities[gid].xyz * dt;

		Colors[gid].rgb = mix(Colors[gid].rgb, vec3(0.5), pow(smoothstep(0, life, Lives[gid]), 1));
		Colors[gid].a = pow(smoothstep(life, 0, Lives[gid]), 0.3);
	}
	Lives[gid] += dt;
}