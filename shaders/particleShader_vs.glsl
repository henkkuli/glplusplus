#version 430

in layout (location = 0) vec4 pos;
in layout (location = 1) vec4 vel;

uniform float time;

out float vage;

void main() {
	gl_Position = vec4(pos.xyz, 1.0);
	vage = vel.w - time;
}