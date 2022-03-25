#pragma once
#include "Chunk.h"
#include <vector>


class Planet {
public:
	Planet();
	Chunk* getChunk(int chunk) const;
	bool isActive(int chunk) const;

private:
	Chunk* chunks[20];
	glm::vec3 icosahedronVertices[12];
	GLuint icosahedronIndices[20][3] = {
   {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
   {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
   {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
   {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} 
	};

	bool activeChunks[20];

	void generateIcosahedronVertices();
};