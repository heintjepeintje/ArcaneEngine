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

layout (std140, binding = 1) uniform LightData {
	PointLight PointLights[32];
	uint PointLightCount;
	DirectionalLight DirLight;
} uLights;

const float PI = 3.1415926535;
const float MIN_SHADOW_BIAS = 0.05;
const float MAX_SHADOW_BIAS = 0.005;

layout (location = 0) in vec2 iUV;

layout (location = 0) out vec3 oColor;

layout (binding = 0) uniform sampler2D uPosition;
layout (binding = 1) uniform sampler2D uAlbedo;
layout (binding = 2) uniform sampler2D uNormal;
layout (binding = 3) uniform sampler2D uMRA;

layout (binding = 4) uniform sampler2D uShadowMapPos;
layout (binding = 5) uniform sampler2D uShadowMap;

vec3 FresnelSchlick(float cosTheta, vec3 f0);
float DistributionGGX(vec3 n, vec3 h, float roughness);
float GeometrySchlickGGX(float ndotv, float roughness);
float GeometrySmith(vec3 n, vec3 v, vec3 l, float roughness);
float GetShadow(float bias, vec3 posLightSpace);

vec3 GetPointLightColor(uint lightIndex, vec3 position, vec3 albedo, vec3 normal, float metallic, float roughness);
vec3 GetDirectionalLightColor(vec3 position, vec3 albedo, vec3 normal, float metallic, float roughness);

void main() {
	const vec3 position = texture(uPosition, iUV).rgb;
	const vec3 albedo = texture(uAlbedo, iUV).rgb;
	const vec3 normal = texture(uNormal, iUV).rgb;
	const float metallic = texture(uMRA, iUV).r;
	const float roughness = texture(uMRA, iUV).g;
	const float ao = texture(uMRA, iUV).b;
	const vec3 shadowMapPos = texture(uShadowMapPos, iUV).rgb;
	
	vec3 result = vec3(0.0);

	for (uint i = 0; i < uLights.PointLightCount; i++) {
		result += GetPointLightColor(i, position, albedo, normal, metallic, roughness);
	}

	const float shadowBias = max(0.05 * (1.0 - dot(normal, -uLights.DirLight.Direction.xyz)), 0.005);
	vec3 lightSpacePos = shadowMapPos * 0.5 + 0.5;
	const float shadow = GetShadow(shadowBias, lightSpacePos);

	result += (1.0 - shadow) * GetDirectionalLightColor(position, albedo, normal, metallic, roughness);

	const vec3 ambient = vec3(0.03) * albedo * ao;
	oColor = ambient + result;
}

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

float GetShadow(float bias, vec3 lightSpacePos) {
	const float currentDepth = lightSpacePos.z;

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(uShadowMap, 0);

	for (int x = -1; x <= 1; ++x) {
		for (int y = -1; y <= 1; y++) {
			float pcfDepth = texture(uShadowMap, lightSpacePos.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}

	return shadow / 9.0;
}

vec3 GetPointLightColor(uint lightIndex, vec3 position, vec3 albedo, vec3 normal, float metallic, float roughness) {
	const vec3 f0 = mix(vec3(0.04), albedo, metallic);
	const vec3 v = normalize(uCamera.Position.xyz - position);

	const vec3 l = normalize(uLights.PointLights[lightIndex].Position.xyz - position);
	const vec3 h = normalize(v + l);

	const float distance = length(uLights.PointLights[lightIndex].Position.xyz - position);
	const float attenuation = 1.0 / (distance * distance);
	const vec3 radiance = uLights.PointLights[lightIndex].Color.rgb * attenuation * uLights.PointLights[lightIndex].Intensity;

	const float ndf = DistributionGGX(normal, h, roughness);
	const float g = GeometrySmith(normal, v, l, roughness);
	const vec3 f = FresnelSchlick(max(dot(h, v), 0.0), f0);

	const vec3 ks = f;
	const vec3 kd = (vec3(1.0) - ks) * (1.0 - metallic);

	const vec3 num = ndf * g * f;
	const float denom = 4.0 * max(dot(normal, v), 0.0) * max(dot(normal, l), 0.0) + 0.0001;
	const vec3 specular = num / denom;

	const float ndotl = max(dot(normal, l), 0.0);
	// return position;
	return (kd * albedo / PI + specular) * radiance * ndotl;
}

vec3 GetDirectionalLightColor(vec3 position, vec3 albedo, vec3 normal, float metallic, float roughness) {
	const vec3 f0 = mix(vec3(0.04), albedo, metallic);
	const vec3 v = normalize(uCamera.Position.xyz - position);

	const vec3 l = normalize(-uLights.DirLight.Direction.xyz);
	const vec3 h = normalize(v + l);

	const float ndf = DistributionGGX(normal, h, roughness);
	const float g = GeometrySmith(normal, v, l, roughness);
	const vec3 f = FresnelSchlick(max(dot(h, v), 0.0), f0);

	const vec3 ks = f;
	const vec3 kd = (vec3(1.0) - ks) * (1.0 - metallic);

	const vec3 num = ndf * g * f;
	const float denom = 4.0 * max(dot(normal, v), 0.0) * max(dot(normal, l), 0.0) + 0.0001;
	const vec3 specular = num / denom;

	const float ndotl = max(dot(normal, l), 0.0);
	const vec3 radiance = uLights.DirLight.Color.xyz * ndotl;

	return (kd * albedo / PI + specular) * radiance * 5.0;
}