#version 330

flat in uint grid;
flat in uint x;
flat in uint y;
flat in uint z;

//out uvec4 FragColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(float(grid), float(x), float(y), float(z));
    //FragColor = uvec4(grid, x, y, z);
   // FragColor = uvec4(0.5, 0.5, 0.5, 0.6);
}