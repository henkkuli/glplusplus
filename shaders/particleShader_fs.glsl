#version 430

out vec4 fragColor;

in float fage;

void main() {
	fragColor = mix(vec4(1, 0, 0, 0.01), vec4(0.5, 0.25, 0, 0.01), fage);
}