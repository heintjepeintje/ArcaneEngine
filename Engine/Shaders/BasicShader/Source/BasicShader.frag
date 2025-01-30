#version 430 core

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iUV;
layout (location = 3) in vec3 iTangent;
layout (location = 4) in vec3 iBitangent;

layout (location = 0) out vec4 oColor;

layout (binding = 0) uniform sampler2D uSampler;

void main() {
	oColor = texture2D(uSampler, iUV);
}