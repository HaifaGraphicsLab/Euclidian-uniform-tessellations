#include "Chunk.h"
#include <iostream>

PixelInfo createPixelInfo(const Voxel& v, int neighbor) {
	PixelInfo p;
	p.gridAndNeighbor = (neighbor << 16) | v.grid;
	p.x = v.x + 1;
	p.y = v.y + 1;
	p.z = v.z + 1;
	return p;
}
Voxel extractPixelInfo(const PixelInfo& pi, bool* isSuccessful, int* neighbor) {
	int n = pi.gridAndNeighbor >> 16;
	int grid = pi.gridAndNeighbor & 0x0000FFFF;
	if (grid == 6) {
		*isSuccessful = false;
		return { 6, 0, 0, 0 };
	}
	else {
		*isSuccessful = true;
	}
	Voxel v;
	v.grid = grid;
	*neighbor = n;
	v.x = pi.x - 1;
	v.y = pi.y - 1;
	v.z = pi.z - 1;
	return v;
}
Chunk::Chunk(ChunkBorder borders, ChunkLoc loc, Grid* grid)
{
	this->borders = borders;
	this->loc = loc;
	this->grid = grid;
	this->size = grid->getY(); // size is as the grid height
	this->update = true;

	glGenVertexArrays(1, &vao);
	glGenVertexArrays(1, &pickerVao);
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &pickerVbo);

}

ChunkBorder Chunk::getBorders() const
{
	return borders;
}

GLuint Chunk::getVAO()
{
	return vao;
}

GLuint Chunk::getPickerVAO()
{
	return pickerVao;
}


GLuint Chunk::getVBO()
{
	return vbo;
}

GLuint Chunk::getPickerVBO()
{
	return pickerVbo;
}

GLsizei Chunk::getNumOfVertices() const
{
	return vertexArray.size();
}

ChunkLoc Chunk::getLoc() const
{
	return loc;
}
