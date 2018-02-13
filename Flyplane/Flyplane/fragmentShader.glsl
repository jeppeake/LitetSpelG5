#version 420
uniform sampler2D texSampler;
uniform sampler2D shadowMap;
uniform mat4 shadowMatrix;
uniform vec3 cameraPos;

in vec3 Pos;
in vec3 Normal;
in vec2 Tex;


void main() {
	vec3 shadowCoord = (shadowMatrix * vec4(Pos, 1)).xyz;
	float depth = texture(shadowMap, shadowCoord.xy).r;
	float visibility = 1.0;
	if(depth < shadowCoord.z - 0.001) {
		visibility = 0.0;
	}
	float x = shadowCoord.x;
	float y = shadowCoord.y;
	if(x < 0 || x > 1 || y < 0 || y > 1) {
		visibility = 1.0;
	}


	vec3 color = texture(texSampler, vec2(Tex.x, 1 - Tex.y)).rgb;


	vec3 sun = normalize(vec3(0, 1, 1));
	vec3 n = normalize(Normal);

	float diffuse = dot(sun, n);
	diffuse = clamp(diffuse, 0, 1);
	
	vec3 look = normalize(cameraPos - Pos);
	vec3 half = normalize(look + sun);
	float specular = pow(clamp(dot(n, half), 0.0, 1.0), 100.0);

	vec3 lighting;
	lighting += color * diffuse * visibility * 0.7;
	lighting += color * specular * visibility;
	lighting += color * 0.3;
	gl_FragColor = vec4(lighting, 1);
	
}