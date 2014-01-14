#version 430

#define PARTICLE_WIDTH 0.1
#define PARTICLE_HEIGHT 0.1

layout (points) in;

layout (triangle_strip, max_vertices = 4) out;

in float vage[];
out float fage;
out vec2 pos;
//uniform ve2 screenDimensions;

void main() {
	for (int i = 0; i < gl_VerticesIn; ++i) {
		gl_Position = gl_in[i].gl_Position;
		//gl_Position.z = 0;
		//gl_Position.w = 1;

		fage = vage[i];
		//fage = gl_Position.w;

		gl_Position.xy -= vec2(PARTICLE_WIDTH/2, PARTICLE_HEIGHT/2);
		pos = vec2(-1,-1);
		EmitVertex();
		gl_Position.x += PARTICLE_WIDTH;
		pos = vec2(1, -1);
		EmitVertex();
		gl_Position.x -= PARTICLE_WIDTH;
		gl_Position.y += PARTICLE_HEIGHT;
		pos = vec2(-1, 1);
		EmitVertex();
		gl_Position.x += PARTICLE_WIDTH;
		pos = vec2(1, 1);
		EmitVertex();
	}
}