#version 330 core

in vec3 fragPos;
in vec4 vertexColor;
// The final color of the fragment (pixel)
out vec4 FragColor;


void main()
{
	FragColor = vertexColor;
}
