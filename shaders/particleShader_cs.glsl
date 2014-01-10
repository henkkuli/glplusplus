#version 430

#define GRAVITY vec4(0, 0, 0, 0)

struct particle {
	vec4 pos;
	vec4 vel;		// vel.w contains death time
};

layout(std430, binding = 0) buffer pos {
	particle particles[];
};

uniform float time;
uniform float dtime;

layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;

void main() {
	uint i = gl_GlobalInvocationID.x;
	particle p = particles[i];

	if (p.vel.w <= time) {
		p.pos = vec4(0, 0, 0, 1);
		p.vel = vec4((mod(time, 0.1)-0.05)*0.3, 0.1+cos(time*1024)*0.1, 0, time+1+fract(dtime*i));
	}
	p.pos.xyz += p.vel.xyz * dtime;
	p.vel += GRAVITY * dtime;

	particles[i] = p;
}