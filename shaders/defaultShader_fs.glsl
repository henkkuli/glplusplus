#version 430

uniform sampler2D sampler;

in vec2 textureCoordinates;
out vec4 fragColor;

void main() {
	fragColor = vec4(texture2D(sampler, textureCoordinates).rgb, 1.0);
}