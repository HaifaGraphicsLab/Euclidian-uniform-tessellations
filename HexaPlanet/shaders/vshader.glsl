#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in int colorIndex;
layout(location = 2) in int ambientOcclusion;

const vec3 colorArray[16] = vec3[](
vec3(0, 0, 0) / 255.0f,
vec3(64, 64, 64) / 255.0f,
vec3(255, 0, 0) / 255.0f,
vec3(255, 106, 0) / 255.0f,
vec3(255, 216, 0) / 255.0f,
vec3(118, 255, 0) / 255.0f,
vec3(0, 255, 144) / 255.0f,
vec3(0, 163, 33) / 255.0f,
vec3(76, 255, 0) / 255.0f,
vec3(0, 255, 255) / 255.0f,
vec3(0, 148, 255) / 255.0f,
vec3(0, 38, 255) / 255.0f,
vec3(178, 0, 255) / 255.0f,
vec3(255, 0, 220) / 255.0f,
vec3(255, 0, 110) / 255.0f,
vec3(255, 255, 255) / 255.0f

);

uniform mat4 view;
uniform mat4 projection;

// These outputs will be available in the fragment shader as inputs
out vec3 fragPos;
out vec4 vertexColor;
// out vec3 fragNormal;

void main()
{
	fragPos = vec3(vec4(pos, 1.0f));
	gl_Position = projection * view * vec4(pos, 1.0f);


	// This is an internal OpenGL variable, we must set a value to this variable
	float alpha = 1-ambientOcclusion/4.0;

	vec3 color = colorArray[colorIndex];
	vertexColor = vec4(color, 1.0) * alpha;
}