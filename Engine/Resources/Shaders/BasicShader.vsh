#version 430 core

out FS_OUT {
	vec4 color;
} fsOutput;

void main() {
	fsOutput.color = vec4(1.0, 0.0, 0.0, 1.0);
}