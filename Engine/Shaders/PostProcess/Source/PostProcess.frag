#version 430 core

layout (location = 0) in vec2 iUV;

layout (location = 0) out vec3 oColor;

layout (binding = 0) uniform sampler2D uTexture;

layout (std140, binding = 0) uniform PostProcessSettings {
	float Gamma;
	float Exposure;
} uSettings;

void main() {
	vec3 color = texture(uTexture, iUV).rgb;

	// Exposure
	vec3 mapped = vec3(1.0) - exp(-color * uSettings.Exposure);

	// Gamma
	if (uSettings.Gamma != 0.0) {
		mapped = pow(mapped, vec3(1.0 / uSettings.Gamma));
	}
	oColor = color;
}