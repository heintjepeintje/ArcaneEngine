#version 430 core

layout (location = 0) in vec2 iUV;
layout (location = 1) in vec4 iColor;

layout (location = 0) out vec4 oColor;

layout (binding = 0) uniform sampler2D uTexture;

void main() {
	oColor = iColor * texture(uTexture, iUV);
}