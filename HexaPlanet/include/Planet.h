#pragma once
#include "Chunk.h"
#include <vector>
#include "Grid.h"



class Planet {
public:
	Planet(int size, int maxHeight);
	Chunk* getChunk(int chunk) const;
	bool isActive(int chunk) const;
	bool activeChunks[20];
	Voxel getNeighbor(Voxel v, int neighbor) const;
	ChunkLoc GetChunkLoc(Voxel v) const;
	glm::vec3 voxelTo3DCoords(Voxel v) const;
	GLuint getVAO(int chunk);

private:
	Chunk* chunks[20];

	glm::vec3 icosahedronVertices[12];
	GLuint icosahedronIndices[20][3] = {
		{0,1,2}, {0,2,3}, {0,3,4}, {0,4,5}, {0,5,1},
		{2,1,7}, {3,2,8}, {4,3,9}, {5,4,10}, {1,5,6},
		{1,6,7},{2,7,8},{3,8,9}, {4,9,10}, {5,10,6},
		{6,7,11}, {7,8,11}, {8,9,11}, {9,10,11}, {10,6,11}
	};

	
	Grid *grid[5];
	int size;
	int maxHeight;
	int neighbors[8][3] = { {0, -1, 0}, {1, -1, 0}, {1, 0, 0}, {0, 1, 0}, {-1, 1, 0}, {-1, 0, 0} , {0,0,-1}, {0,0,1} };


	void generateIcosahedronVertices();
	float heightFunc(int z) const;
	glm::vec2 gridTo2DCoords(int x, int y) const;
	glm::vec3 gridToBarycentric(int x, int y, int index) const;
	glm::vec3 barycentric(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 p) const;
	void renderHex(Voxel v) const;
	void updateVertexArray(int chunk);
};