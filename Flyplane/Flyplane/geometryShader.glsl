layout(triangles) in;
layout(triangles, max_vertices = 3) out;

in vec3 Pos[];
in vec3 Normal[];
in vec2 Tex[];

out vec3 vPos;
out vec3 vNormal;
out vec2 vTex;

void main() {
	vec3 v1 = Pos[0] - Pos[1];
	vec3 v2 = Pos[0] - Pos[2];
	vNormal = cross(v1, v2);
	for(int i = 0; i < 3; i++) {
		vPos = Pos[i];
		vTex = Tex[i];
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
	
}

