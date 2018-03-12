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
uniform vec3 velocity;
layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;
float rand(float n) { 
	return 2.0*fract(sin(n * 12.9898) * 43758.5453)-1.0;
}
void main()
{
	uint gid = gl_GlobalInvocationID.x;
	if(Positions[gid].xyz == vec3(0))
	{
		// INITAL SPAWN
		Positions[gid].a = 1.0;
		Lives[gid] = 0;
		Colors[gid].xyz = vec3(1.0,1.0,0.2);

		// spark count
		float sc = 60;

		vec3 vel;
		vel.x = rand(mod(gid, sc) + 60 * dt);
		vel.y = rand(mod(gid, sc) + 50 + 160 * dt);
		vel.z = rand(mod(gid, sc) + 100 + 260 * dt);
		vel = normalize(vel);

		vec3 pos = 2.5f * abs(rand(2*gid - 1000)) * vel;

		Positions[gid].xyz = spawn + pos;

		Velocities[gid].xyz = velocity + 22*(2 + abs(rand(mod(gid, sc) - 100 * dt))) * vel;
	}
	else
	{
		// UPDATE

		Velocities[gid].y += -9.82 * dt;
		//Velocities[gid].xyz -= Velocities[gid].xyz * (1 - pow(0.1, dt));
		Positions[gid].xyz += Velocities[gid].xyz * dt;

		//Colors[gid].a = pow(smoothstep(life, 0, Lives[gid]), 20);
		Positions[gid].w = pow(smoothstep(life, 0, Lives[gid]), 20);
		
	}
	Lives[gid] += dt;
}