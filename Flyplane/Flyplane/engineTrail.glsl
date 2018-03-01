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
uniform vec3 direction;

layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;

// [-1, 1]
float rand(float n) { 
	return 2.0*fract(sin(n * 12.9898) * 43758.5453)-1.0;
}

void main()
{
	uint gid = gl_GlobalInvocationID.x;
	if(Positions[gid].xyz == vec3(0))
	{
		// INITAL SPAWN

		Positions[gid].xyz = spawn;
		Lives[gid] = life * (rand(float(gid) + 1000) + 1)/2;
		Colors[gid].rgb = vec3(1.0,0.5,0.0);
	}
	else
	{
		// UPDATE
		Colors[gid].a = 0.3*pow(1-Lives[gid] / life, 4);

		Positions[gid].xyz = spawn + Velocities[gid].xyz + direction * (Lives[gid]*20 + 2);
	}
	Lives[gid] += dt;
	if(Lives[gid] >= life)
	{
		// RESPAWN
		Lives[gid] -= life;

		vec3 d = normalize(direction);
		vec3 ortho = normalize(vec3(-d.y, d.x, 0));
		vec3 ortho2 = cross(d, ortho);

		vec3 offset;
		offset += ortho * rand(gid * 7);
		offset += ortho2 * rand(gid * 13);

		offset = 0.2*rand(gid * 19)*normalize(offset);

		Velocities[gid].xyz = offset;

		Positions[gid].xyz = spawn + offset; 

		//Lives[gid] = dt * life * (rand(float(gid) + 1000 + dt) + 1)/2;
	}
}

