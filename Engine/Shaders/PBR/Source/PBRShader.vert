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
layout (location = 5) out mat3 oTBN;

layout (std140, binding = 0) uniform CameraData {
	mat4 Projection;
	mat4 View;
	vec4 Position;
} uCamera;

layout (std140, binding = 1) uniform ObjectData {
	mat4 Model;
	mat4 MVP;
	vec4 Position;
} uObject;

void main() {
	oPosition = (uObject.Model * vec4(aPosition, 1.0)).xyz;
	oNormal = mat3(transpose(inverse(uObject.Model))) * aNormal;
	oUV = aUV;
	oTangent = aTangent;
	oBitangent = aBitangent;

	const vec3 t = normalize(vec3(uObject.Model * vec4(aTangent, 0.0)));
	const vec3 b = normalize(vec3(uObject.Model * vec4(aBitangent, 0.0)));
	const vec3 n = normalize(vec3(uObject.Model * vec4(aNormal, 0.0)));
	oTBN = mat3(t, b, n);

	gl_Position = uObject.MVP * vec4(aPosition, 1.0);
}