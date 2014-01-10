#version 430

in vec4 vp;

void main() {
	gl_Position = vec4(vp.xyz, 1.0);
	gl_PointSize = 15;
}