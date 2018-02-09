#version 420
layout(points) in;
layout(line_strip, max_vertices = 2) out;

uniform vec3 cameraPos;
uniform mat4 aspectMatrix;

in vec3 Pos[];

void main() {
	vec3 dir = cameraPos - Pos[0];
	vec3 up = vec3(0, 1, 0);
	dir = normalize(dir);
	vec3 right = cross(dir, up);
	right = normalize(right);
	up = cross(dir, right);
	up = normalize(up);
	float scale = 10.0;

	/*gl_Position = aspectMatrix * vec4((Pos[0] + right * scale + up * scale));
	EmitVertex();				 										
								 										
	gl_Position = aspectMatrix * vec4((Pos[0] + right * scale - up * scale));
	EmitVertex(); 				 										
	EndPrimitive();				 										
				  				 										
	gl_Position = aspectMatrix * vec4((Pos[0] + right * scale - up * scale));
	EmitVertex(); 														
				  														
	gl_Position = aspectMatrix * vec4((Pos[0] - right * scale - up * scale));
	EmitVertex(); 				 										
	EndPrimitive();				 										
				  				 										
	gl_Position = aspectMatrix * vec4((Pos[0] - right * scale + up * scale));
	EmitVertex(); 				 										
				  				 										
	gl_Position = aspectMatrix * vec4((Pos[0] - right * scale - up * scale));
	EmitVertex(); 				 										
	EndPrimitive();				 										

	gl_Position = aspectMatrix * vec4((Pos[0] - right * scale + up * scale));
	EmitVertex(); 				 										

	gl_Position = aspectMatrix * vec4((Pos[0] + right * scale + up * scale));
	EmitVertex();
	EndPrimitive();

	gl_Position = vec4(0,0,0,1);
	EmitVertex(); 				 										

	gl_Position = vec4(0,1,0,1);
	EmitVertex();
	EndPrimitive();*/
}