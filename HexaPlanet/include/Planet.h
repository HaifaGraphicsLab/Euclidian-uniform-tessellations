#pragma once
#include "Chunk.h"
#include <vector>
#include "Grid.h"

class Planet {
public:
	Planet(int size, int maxHeight);
	Chunk* getChunk(int chunk) const;
	bool isActive(int chunk) const;

private:
	Chunk* chunks[20];
	bool activeChunks[20];

	glm::vec3 icosahedronVertices[12];
	GLuint icosahedronIndices[20][3] = {
		{0,1,2}, {0,2,3}, {0,3,4}, {0,4,5}, {0,5,1},
		{1,2,7}, {2,3,8}, {4,3,9}, {5,4,10}, {1,5,6},
		{1,6,7},{2,7,8},{3,8,9}, {4,9,10}, {5,10,6},
		{6,7,11}, {7,8,11}, {8,9,11}, {9,10,11}, {10,6,11}
	};

	
	Grid *grid[5];
	int size;
	int maxHeight;

	void generateIcosahedronVertices();
};