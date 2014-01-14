#version 430

in layout (location = 0) vec3 pos;

uniform float time;
uniform mat4 proj;
uniform mat4 view;

void main() {
	gl_Position = proj*view*vec4(pos, 1.0);
}