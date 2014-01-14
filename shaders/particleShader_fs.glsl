#version 430

out vec4 fragColor;

in float fage;
in vec2 pos;

void main() {
	fragColor = vec4(mix(vec3(1, 0, 0), vec3(0.5, 0.25, 0), fage) * (1-length(pos)), 0.1);
}