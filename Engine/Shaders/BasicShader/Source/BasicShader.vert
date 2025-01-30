#version 430 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

layout (location = 0) out vec3 oPosition;
layout (location = 1) out vec3 oNormal;
layout (location = 2) out vec2 oUV;
layout (location = 3) out vec3 oTangent;
layout (location = 4) out vec3 oBitangent;

layout (std140, binding = 0) uniform Camera {
	mat4 Projection;
	mat4 View;
} uCamera;

layout (std140, binding = 1) uniform Object {
	mat4 Model;
} uObject;

layout (std140, binding = 2) uniform CoordinateSystem {
	mat4 Convert;
} uCS;

void main(void) {
	oPosition = aPosition;
	oUV = aUV;
	oTangent = aTangent;
   	oBitangent = aBitangent;
   	oNormal = aNormal;

	gl_Position = uCamera.Projection * uCamera.View * uObject.Model * vec4(aPosition, 1.0);
}