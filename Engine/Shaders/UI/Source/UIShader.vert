#version 430 core

layout (location = 0) in vec2 iPosition;
layout (location = 1) in vec2 iUV;
layout (location = 2) in vec4 iColor;

layout (location = 0) out vec2 oUV;
layout (location = 1) out vec4 oColor;

layout (std140, binding = 0) uniform CameraData {
	mat4 Projection;
} uCamera;

void main() {
	oUV = iUV;
	oColor = iColor;
	gl_Position = uCamera.Projection * vec4(iPosition, 0.0, 1.0);
}