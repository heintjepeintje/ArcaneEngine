#version 430 core

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iUV;
layout (location = 3) in vec4 iColor;

layout (location = 0) out vec4 oColor;

void main() {
	oColor = iColor;
}