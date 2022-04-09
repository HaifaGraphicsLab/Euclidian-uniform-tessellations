#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in int colorIndex;
layout(location = 2) in int ambientOcclusion;

const vec3 colorArray[5] = vec3[5]( vec3(1, 0, 0),
vec3(0, 1, 0),
vec3(0, 0, 1),
vec3(1, 1, 0),
vec3(0, 1, 1)
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