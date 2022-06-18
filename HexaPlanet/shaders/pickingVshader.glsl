#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in uint v_grid;
layout(location = 2) in uint v_x;
layout(location = 3) in uint v_y;
layout(location = 4) in uint v_z;

flat out uint grid;
flat out uint x;
flat out uint y;
flat out uint z;

uniform mat4 view;
uniform mat4 projection;

// no outputs needed for fs

void main()
{
	gl_Position = projection * view * vec4(pos, 1.0f);
	grid = v_grid;
	x = v_x;
	y = v_y;
	z = v_z;
}