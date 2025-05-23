#version 430 core

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aUV;

layout (location = 0) out vec2 oUV;

void main() {
	oUV = aUV;
	gl_Position = vec4(aPosition, 0.0, 1.0);
}