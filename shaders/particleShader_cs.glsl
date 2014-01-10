#version 430

struct particle {
	vec4 pos;
	vec4 vel;
};

layout(std430, binding = 0) buffer pos {
	particle particles[];
};

layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;

void main() {
	uint i = gl_GlobalInvocationID.x;
	particle p = particles[i];
	p.pos += p.vel*0.0001f;
	p.vel += vec4(0, -0.001, 0, 0);
	particles[i] = p;
	//positions[i].x += 0.0001*(i+1);// velocities[i].x/100;
}