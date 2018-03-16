#version 420
layout(points) in;
layout(triangle_strip, max_vertices = 3) out;

uniform vec3 cameraPos;
uniform mat4 aspectMatrix;
uniform float scale;

in vec3 Pos[];

void main() {
	vec4 posCS  = aspectMatrix * vec4(Pos[0], 1);
	posCS = posCS / posCS.w;

	if (posCS.z < 1.0) {
		float posY = posCS.y;
		if (posCS.x < -1.0) {
			if (posCS.y < -1.0) {
				posY = -0.97;
			}
	
			else if (posCS.y > 1.0) {
				posY = 0.97;
			}
				
			gl_Position = vec4(-1.0, posY, 0.0, 1.0);
			EmitVertex();

			gl_Position = vec4(-0.97, posY + 0.03, 0.0, 1.0);
			EmitVertex();

			gl_Position = vec4(-0.97, posY - 0.03, 0.0, 1.0);
			EmitVertex();
			EndPrimitive();
		}

		else if (posCS.x > 1.0) {
			if (posCS.y < -1.0) {
				posY = -0.97;
			}
	
			else if (posCS.y > 1.0) {
				posY = 0.97;
			}
				
			gl_Position = vec4(1.0, posY, 0.0, 1.0);
			EmitVertex();

			gl_Position = vec4(0.97, posY + 0.03, 0.0, 1.0);
			EmitVertex();

			gl_Position = vec4(0.97, posY - 0.03, 0.0, 1.0);
			EmitVertex();
			EndPrimitive();
		}

		else if (posCS.y < -1.0) {
			gl_Position = vec4(posCS.x, -1.0, 0.0, 1.0);
			EmitVertex();

			gl_Position = vec4(posCS.x - 0.03, -0.97, 0.0, 1.0);
			EmitVertex();

			gl_Position = vec4(posCS.x + 0.03, -0.97, 0.0, 1.0);
			EmitVertex();
			EndPrimitive();
		}

		else if (posCS.y > 1.0) {
			gl_Position = vec4(posCS.x, 1.0, 0.0, 1.0);
			EmitVertex();

			gl_Position = vec4(posCS.x - 0.03, 0.97, 0.0, 1.0);
			EmitVertex();

			gl_Position = vec4(posCS.x + 0.03, 0.97, 0.0, 1.0);
			EmitVertex();
			EndPrimitive();
		}
	}

	else{
		float posY = -posCS.y;
		if (posCS.x < -1.0) {
			if (posCS.y < -1.0) {
				posY = 1;
			}
			else if (posCS.y > 1.0) {
				posY = -1;
			}
			gl_Position = vec4(1.0, posY, 0.0, 1.0);
			EmitVertex();

			gl_Position = vec4(0.97, posY + 0.03, 0.0, 1.0);
			EmitVertex();

			gl_Position = vec4(0.97, posY - 0.03, 0.0, 1.0);
			EmitVertex();
			EndPrimitive();
		}
		else if (posCS.x > 1.0) {
			if (posCS.y < -1.0) {
				posY = 1;
			}
			else if (posCS.y > 1.0) {
				posY = -1;
			}
			gl_Position = vec4(-1.0, posY, 0.0, 1.0);
			EmitVertex();

			gl_Position = vec4(-0.97, posY + 0.03, 0.0, 1.0);
			EmitVertex();

			gl_Position = vec4(-0.97, posY - 0.03, 0.0, 1.0);
			EmitVertex();
			EndPrimitive();
		}
		else if (posCS.y < -1.0) {
			gl_Position = vec4(-posCS.x, 1.0, 0.0, 1.0);
			EmitVertex();

			gl_Position = vec4(-posCS.x - 0.03, 0.97, 0.0, 1.0);
			EmitVertex();

			gl_Position = vec4(-posCS.x + 0.03, 0.97, 0.0, 1.0);
			EmitVertex();
			EndPrimitive();
		}
		else if (posCS.y > 1.0) {
			gl_Position = vec4(-posCS.x, -1.0, 0.0, 1.0);
			EmitVertex();

			gl_Position = vec4(-posCS.x - 0.03, -0.97, 0.0, 1.0);
			EmitVertex();

			gl_Position = vec4(-posCS.x + 0.03, -0.97, 0.0, 1.0);
			EmitVertex();
			EndPrimitive();
		}
		else {
			float temp = 1;
			if (posY < 0.0) {
				temp = -1;
			}
			gl_Position = vec4(-posCS.x, temp * 1.0, 0.0, 1.0);
			EmitVertex();

			gl_Position = vec4(-posCS.x - 0.03, temp * 0.97, 0.0, 1.0);
			EmitVertex();

			gl_Position = vec4(-posCS.x + 0.03, temp * 0.97, 0.0, 1.0);
			EmitVertex();
			EndPrimitive();
		}
	}
}