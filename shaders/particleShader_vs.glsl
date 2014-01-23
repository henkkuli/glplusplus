#version 430

in layout (location = 0) vec4 pos;
in layout (location = 1) vec4 vel;

uniform float time;
uniform mat4 proj, view;

out float vage;

void main() {
	gl_Position = proj * view * vec4(pos.xyz, 1.0);
	vage = clamp(vel.w, 0, 1);
}