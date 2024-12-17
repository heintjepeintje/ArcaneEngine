#version 430 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec4 aColor;

layout (location = 0) out vec3 oPosition;
layout (location = 1) out vec3 oNormal;
layout (location = 2) out vec2 oUV;
layout (location = 3) out vec4 oColor;

layout (std140, binding = 0) uniform Camera {
	mat4 Projection;
} uCamera;

void main(void) {
	oPosition = aPosition;
	oNormal = aNormal;
	oUV = aUV;
	oColor = aColor;

	gl_Position = uCamera.Projection * vec4(aPosition, 1.0);
}