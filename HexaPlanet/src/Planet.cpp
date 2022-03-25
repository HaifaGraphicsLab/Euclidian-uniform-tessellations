#include "Planet.h"

// constants
#define X .525731112119133606
#define Z .850650808352039932

Planet::Planet()
{
	this->generateIcosahedronVertices();
	
	for (int i = 0; i < 20; i++) {
		glm::vec3 borders[3];
		borders[0] = icosahedronVertices[icosahedronIndices[i][0]];
		borders[1] = icosahedronVertices[icosahedronIndices[i][1]];
		borders[2] = icosahedronVertices[icosahedronIndices[i][2]];

		chunks[i] = new Chunk(borders[0], borders[1], borders[2]);
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
	this->icosahedronVertices[0] = glm::vec3(-X, 0.0, Z);
	this->icosahedronVertices[1] = glm::vec3(X, 0.0, Z);
	this->icosahedronVertices[2] = glm::vec3(-X, 0.0, -Z);
	this->icosahedronVertices[3] = glm::vec3(X, 0.0, -Z);
	this->icosahedronVertices[4] = glm::vec3(0.0, Z, X);
	this->icosahedronVertices[5] = glm::vec3(0.0, Z, -X);
	this->icosahedronVertices[6] = glm::vec3(0.0, -Z, X);
	this->icosahedronVertices[7] = glm::vec3(0.0, -Z, -X);
	this->icosahedronVertices[8] = glm::vec3(Z, X, 0.0);
	this->icosahedronVertices[9] = glm::vec3(-Z, X, 0.0);
	this->icosahedronVertices[10] = glm::vec3(Z, -X, 0.0);
	this->icosahedronVertices[11] = glm::vec3(-Z, -X, 0.0);
}

