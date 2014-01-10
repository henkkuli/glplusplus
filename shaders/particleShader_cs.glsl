#version 430

layout(std430, binding = 0) buffer pos {
	vec4 positions[];
};

layout(std430, binding = 1) buffer vel {
	vec4 velocities[];
};

layout( local_size_x = 128, local_size_y = 1, local_size_z = 1 ) in;

void main() {
	uint i = gl_GlobalInvocationID.x;
	positions[i].x += 0.0001*(i+1);// velocities[i].x/100;
}