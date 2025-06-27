#version 430 core

precision highp float;

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iLightSpacePos;
layout (location = 2) in vec3 iNormal;
layout (location = 3) in vec2 iUV;
layout (location = 4) in vec3 iTangent;
layout (location = 5) in vec3 iBitangent;
layout (location = 6) in mat3 iTBN;

layout (binding = 0) uniform sampler2D uAlbedo;
layout (binding = 1) uniform sampler2D uNormal;
layout (binding = 2) uniform sampler2D uMetallic;
layout (binding = 3) uniform sampler2D uRoughness;
layout (binding = 4) uniform sampler2D uAmbientOcclusion;

layout (location = 0) out vec3 oPosition;
layout (location = 1) out vec4 oAlbedo;
layout (location = 2) out vec3 oNormal;
layout (location = 3) out vec3 oMRA;
layout (location = 4) out vec3 oLightSpacePos;

void main() {
	oPosition = vec3(1.0, 0.0, 0.0);
	oAlbedo = texture(uAlbedo, iUV);
	oNormal = iNormal;
	// oNormal = normalize(iTBN * (texture(uNormal, iUV).rgb * 2.0 - 1.0));
	oMRA = vec3(texture(uMetallic, iUV).r, texture(uRoughness, iUV).r, texture(uAmbientOcclusion, iUV).r);
	oLightSpacePos = iLightSpacePos;
}