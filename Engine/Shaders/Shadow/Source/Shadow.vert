#version 430 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

layout (std140, binding = 0) uniform ObjectData {
	mat4 Model;
	mat4 MVP;
	vec4 Position;
} uObject;

layout (std140, binding = 1) uniform LightData {
	mat4 Projection;
	mat4 View;
} uLight;

void main() {
	gl_Position = uLight.Projection * uLight.View * uObject.Model * vec4(aPosition, 1.0);
}