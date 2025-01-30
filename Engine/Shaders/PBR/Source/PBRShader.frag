#version 430 core

struct PointLight {
	vec4 Position;
	vec4 Color;
	float Intensity;
};

struct DirectionalLight {
	vec4 Direction;
	vec4 Color;
};

layout (std140, binding = 0) uniform CameraData {
	mat4 Projection;
	mat4 View;
	vec4 Position;
} uCamera;

layout (std140, binding = 2) uniform LightData {
	PointLight PointLights[32];
	uint PointLightCount;
	DirectionalLight DirLight;
} uLights;

const float PI = 3.14159265359;

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iUV;
layout (location = 3) in vec3 iTangent;
layout (location = 4) in vec3 iBitangent;
layout (location = 5) in mat3 iTBN;

layout (location = 0) out vec4 oColor;

layout (binding = 0) uniform sampler2D uAlbedo;
layout (binding = 1) uniform sampler2D uNormal;
layout (binding = 2) uniform sampler2D uMetallic;
layout (binding = 3) uniform sampler2D uRoughness;
layout (binding = 4) uniform sampler2D uAmbientOcclusion;

vec3 FresnelSchlick(float cosTheta, vec3 f0) {
	return f0 + (1.0 - f0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0); 
}

float DistributionGGX(vec3 n, vec3 h, float roughness) {
	const float a = roughness * roughness;
	const float a2 = a * a;

	const float ndoth = max(dot(n, h), 0.0);
	const float ndoth2 = ndoth * ndoth;

	const float num = a2;
	float denom = (ndoth2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return num / denom;
}

float GeometrySchlickGGX(float ndotv, float roughness) {
	const float r = (roughness + 1.0);
	const float k = (r * r) / 8.0;

	const float num = ndotv;
	const float denom = ndotv * (1.0 - k) + k;

	return num / denom;
}

float GeometrySmith(vec3 n, vec3 v, vec3 l, float roughness) {
	const float ndotv = max(dot(n, v), 0.0);
	const float ndotl = max(dot(n, l), 0.0);
	const float ggx2 = GeometrySchlickGGX(ndotv, roughness);
	const float ggx1 = GeometrySchlickGGX(ndotl, roughness);

	return ggx1 * ggx2;
}

void main() {
	const vec3 albedo = texture(uAlbedo, iUV).rgb;
	const vec3 normal = normalize(iTBN * texture(uNormal, iUV).rgb * 2.0 - 1.0);
	const float metallic = texture(uMetallic, iUV).r;
	const float roughness = texture(uRoughness, iUV).r;
	const float ao = texture(uAmbientOcclusion, iUV).r;

	const vec3 f0 = mix(vec3(0.04), albedo, metallic);

	const vec3 n = normal;
	const vec3 v = normalize(uCamera.Position.xyz - iPosition);
	
	vec3 result = vec3(0.0);

	for (uint i = 0; i < uLights.PointLightCount; i++) {
		const vec3 l = normalize(uLights.PointLights[i].Position.xyz - iPosition);
		const vec3 h = normalize(v + l);

		const float distance = length(uLights.PointLights[i].Position.xyz - iPosition);
		const float attenuation = 1.0 / (distance * distance);
		const vec3 radiance = uLights.PointLights[i].Color.xyz * attenuation * uLights.PointLights[i].Intensity;

		const float ndf = DistributionGGX(n, h, roughness);
		const float g = GeometrySmith(n, v, l, roughness);
		const vec3 f = FresnelSchlick(max(dot(h, v), 0.0), f0);

		const vec3 ks = f;
		const vec3 kd = (vec3(1.0) - ks) * (1.0 - metallic);

		const vec3 num = ndf * g * f;
		const float denom = 4.0 * max(dot(n, v), 0.0) * max(dot(n, l), 0.0) + 0.0001;
		const vec3 specular = num / denom;

		const float ndotl = max(dot(n, l), 0.0);
		result += (kd * albedo / PI + specular) * radiance * ndotl;
	}

	const vec3 ambient = vec3(0.03) * albedo * ao;
	const vec3 color = ambient + result;

	oColor = vec4(color, 1.0);
}