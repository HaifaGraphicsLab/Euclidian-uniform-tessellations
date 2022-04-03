#include "Planet.h"

// constants
#define X .525731112119133606
#define Z .850650808352039932

Planet::Planet(int size, int maxHeight) : size(size), maxHeight(maxHeight)
{
	this->generateIcosahedronVertices();

	// init Grids of root chunks
	for (int i = 0; i < 5; i++) {
		this->grid[i] = new Grid(size * 2, size, maxHeight);
	}

	// create individual chunks
	for (int i = 0; i < 20; i++) {
		ChunkBorder borders;
		borders.a = icosahedronVertices[icosahedronIndices[i][0]];
		borders.b = icosahedronVertices[icosahedronIndices[i][1]];
		borders.c = icosahedronVertices[icosahedronIndices[i][2]];

		ChunkLoc loc;
		loc.chunckRoot = i % 5;
		loc.index = (int)i / 5;
		chunks[i] = new Chunk(borders, loc, grid[loc.chunckRoot]);
	}

	// all chunks start active
	for (int i = 0; i < 20; i++) {
		activeChunks[i] = true;
	}

}

Chunk* Planet::getChunk(int chunk) const
{
	return chunks[chunk];
}

bool Planet::isActive(int chunk) const
{
	return activeChunks[chunk];
}


void Planet::generateIcosahedronVertices()
{
	this->icosahedronVertices[0] = glm::vec3(0, Z, X);
	this->icosahedronVertices[1] = glm::vec3(-X, 0, Z);
	this->icosahedronVertices[2] = glm::vec3(X, 0, Z);
	this->icosahedronVertices[3] = glm::vec3(Z, X, 0);
	this->icosahedronVertices[4] = glm::vec3(0, Z, -X);
	this->icosahedronVertices[5] = glm::vec3(-Z, X, 0);
	this->icosahedronVertices[6] = glm::vec3(-Z, -X, 0);
	this->icosahedronVertices[7] = glm::vec3(0, -Z, X);
	this->icosahedronVertices[8] = glm::vec3(Z, -X, 0);
	this->icosahedronVertices[9] = glm::vec3(X, 0, -Z);
	this->icosahedronVertices[10] = glm::vec3(-X, 0, -Z);
	this->icosahedronVertices[11] = glm::vec3(0, -Z, -X);
}

