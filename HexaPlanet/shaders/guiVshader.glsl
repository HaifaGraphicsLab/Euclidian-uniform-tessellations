#version 330 core

layout(location = 0) in vec2 position;
uniform mat4 transform;
out vec2 TexCoord;

void main(void) {
	gl_Position = transform * vec4(position, 0.0, 1.0);
	TexCoord = vec2((position.x+1.0)/2.0, 1 - (position.y+1.0)/2.0);
}