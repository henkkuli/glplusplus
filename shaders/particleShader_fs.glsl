#version 430

out vec4 fragColor;
uniform sampler2D sampler;

in float fage;
in vec2 pos;

const float elementWidth = 1.0/8;
const float elementHeight = 1.0/8;

void main() {
	const float x = 1-(int(fage*64-1)%8+1)*elementWidth;
	const float y = (int(fage*64-1))/8*elementHeight;
	fragColor = texture2D(sampler, vec2(vec2(x, y) + pos*vec2(elementWidth, elementHeight))) + 0.0000001*vec4(mix(vec3(1, 0, 0), vec3(0.5, 0.25, 0), fage) * (1-length(pos)), 0.1);
}