#version 430 core

precision highp float;

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

layout (location = 0) out vec3 oPosition;
layout (location = 1) out vec3 oLightSpacePos;
layout (location = 2) out vec3 oNormal;
layout (location = 3) out vec2 oUV;
layout (location = 4) out vec3 oTangent;
layout (location = 5) out vec3 oBitangent;
layout (location = 6) out mat3 oTBN;

layout (std140, binding = 0) uniform CameraData {
	mat4 Projection;
	mat4 View;
	vec4 Position;
	float Near, Far;
} uCamera;

layout (std140, binding = 1) uniform ObjectData {
	mat4 Model;
	mat4 MVP;
	vec4 Position;
} uObject;

layout (std140, binding = 2) uniform ShadowData {
	mat4 Projection;
	mat4 View;
} uShadow;

void main() {
	const mat3 modelNoScale = mat3(transpose(inverse(uObject.Model))); 
	oPosition = (uObject.Model * vec4(aPosition, 1.0)).xyz;
	
	vec4 lightSpacePos = uShadow.Projection * uShadow.View * vec4(oPosition, 1.0);
	oLightSpacePos = lightSpacePos.xyz / lightSpacePos.w;

	oUV = aUV;
	
	oNormal = normalize(modelNoScale * aNormal);
	oTangent = normalize(modelNoScale * aTangent);
	oBitangent = normalize(modelNoScale * aBitangent);
	oTBN = mat3(oTangent, oBitangent, oNormal);

	gl_Position = uObject.MVP * vec4(aPosition, 1.0);
}