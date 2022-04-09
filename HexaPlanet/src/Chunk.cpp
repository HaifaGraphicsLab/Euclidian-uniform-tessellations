#include "Chunk.h"
#include <iostream>


Chunk::Chunk(ChunkBorder borders, ChunkLoc loc, Grid* grid)
{
	this->borders = borders;
	this->loc = loc;
	this->grid = grid;
	this->size = grid->getY(); // size is as the grid height
	this->update = true;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

}

ChunkBorder Chunk::getBorders() const
{
	return borders;
}

GLuint Chunk::getVAO()
{
	return vao;
}

GLuint Chunk::getVBO()
{
	return vbo;
}

GLsizei Chunk::getNumOfVertices() const
{
	return vertexArray.size();
}

ChunkLoc Chunk::getLoc() const
{
	return loc;
}
