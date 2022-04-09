#include "Planet.h"

// constants
#define X .525731112119133606
#define Z .850650808352039932
#include <stdlib.h>     /* srand, rand */
#include <iostream>


Planet::Planet(int size, int maxHeight) : size(size), maxHeight(maxHeight)
{
	this->generateIcosahedronVertices();

	// init Grids of root chunks
	for (int i = 0; i < 5; i++) {
		this->grid[i] = new Grid(size * 2, size, maxHeight);
	}

	// (*grid[0])(0, 0, 3) = BlockType::black;
	//(*grid[0])(0, size - 1, 3) = BlockType::green;
	// (*grid[0])(size-1, 0, 3) = BlockType::red;

	//(*grid[0])(1, size-1, 3) = BlockType::black;
	//(*grid[0])(size-1, size - 1, 3) = BlockType::green;
	// (*grid[0])(size - 1, 1, 3) = BlockType::red;

	//(*grid[0])(size, 0, 3) = BlockType::black;
	//(*grid[4])(1, size-1, 3) = BlockType::black;

	//(*grid[0])(size, size - 1, 3) = BlockType::green;
	//(*grid[0])(2*size - 1, 0, 3) = BlockType::red;


	for (int i = 0; i < 5; i++) {
		for (int x = 0; x < 2 * size; x++) {
			for (int y = 0; y < size; y++) {
				int t = rand() % 3;
				int h = rand() % 1 + 1;
				if (t % 3 == 1) {
					(*grid[i])(x, y, h) = BlockType::black;
				}
				if (t% 3 == 0) {
					(*grid[i])(x, y, h) = BlockType::red;
				}
				if (t % 3 == 2) {
					(*grid[i])(x, y, h) = BlockType::green;
				}
			}
		}
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

Voxel Planet::getNeighbor(Voxel v, int neighbor) const
{
	assert(neighbor >= 0 && neighbor < 8);
	Voxel newV;
	int index = GetChunkLoc(v).index;
	newV.x = v.x + neighbors[neighbor][0];
	newV.y = v.y + neighbors[neighbor][1];
	newV.z = v.z + neighbors[neighbor][2];
	newV.grid = v.grid;
	
	// special cases
	if (newV.x == 0 && newV.y == -1) {
		newV.grid = 0;
		return newV;
	}

	if (newV.x == 2 * size && newV.y == size - 1) {
		newV.grid = 0;
		return newV;
	}

	// breach between root chunks
	if (newV.x >= 2*size) {
		newV.x = size + newV.y+1;
		newV.y = size - 1;
		newV.grid = (newV.grid + 4) % 5;
	}
	if (newV.x < 0) {
		newV.x = newV.y;
		newV.y = 0;
		newV.grid = (newV.grid + 1) % 5;
	}
	if (newV.y >= size) {
		if (index == 3) {
			newV.y = newV.x - size;
			newV.x = 2*size-1;
			newV.grid = (newV.grid + 1) % 5;
		}
		else {
			newV.y = 0;
			newV.x = newV.x + size;
			newV.grid = (newV.grid + 1) % 5;
		}
	}
	if (newV.y < 0) {
		if (index == 0) {
			newV.y = newV.x - 1;
			newV.x = 0;
			newV.grid = (newV.grid + 4) % 5;
		}
		else {
			newV.y = size-1;
			newV.x = newV.x-size;
			newV.grid = (newV.grid + 4) % 5;
		}

	}


	return newV;
}

ChunkLoc Planet::GetChunkLoc(Voxel v) const
{
	ChunkLoc c;
	c.chunckRoot = v.grid;
	if (v.x + v.y < size) {
		c.index = 0;
	}
	else if (v.x < size) {
		c.index = 1;
	}
	else if (v.x + v.y < 2 * size) {
		c.index = 2;
	}
	else {
		c.index = 3;
	}
	return c;
}

glm::vec3 Planet::voxelTo3DCoords(Voxel v) const
{
	ChunkLoc c = GetChunkLoc(v);
	// get barycentiric coordinates
	glm::vec3 barycentric = gridToBarycentric(v.x, v.y, c.index);
	// perform double slerp
	int chunkIndex = c.index * 5 + c.chunckRoot;
	ChunkBorder borders = getChunk(chunkIndex)->getBorders();
	return glm::normalize(barycentric.x * borders.a + barycentric.y * borders.b + barycentric.z * borders.c) * heightFunc(v.z);
	return glm::vec3();
}

GLuint Planet::getVAO(int chunk)
{
	if (chunks[chunk]->update) {
		updateVertexArray(chunk);
	}
	return chunks[chunk]->getVAO();
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

float Planet::heightFunc(int z) const
{
	return size / 2.0f + 20.0f * z / size;
}


glm::vec2 Planet::gridTo2DCoords(int x, int y) const
{
	float x_coord = (sqrt(3) * x + sqrt(3) / 2 * y);
	float y_coord = 3.0 / 2 * y;

	return glm::vec2(x_coord, y_coord);
}

glm::vec3 Planet::gridToBarycentric(int x, int y, int index) const
{
	GridBorder gridBorders;
	switch (index)
	{
	case 0:
		gridBorders.a = gridTo2DCoords(0, -1);
		gridBorders.b = gridTo2DCoords(size, -1);
		gridBorders.c = gridTo2DCoords(0, size - 1);

		break;
	case 1:
		gridBorders.a = gridTo2DCoords(0, size - 1);
		gridBorders.b = gridTo2DCoords(size, -1);
		gridBorders.c = gridTo2DCoords(size, size - 1);
		break;
	case 2:
		gridBorders.b = gridTo2DCoords(2 * size, -1);
		gridBorders.a = gridTo2DCoords(size, -1);
		gridBorders.c = gridTo2DCoords(size, size - 1);
		break;
	case 3:
		gridBorders.b = gridTo2DCoords(size, size - 1);
		gridBorders.a = gridTo2DCoords(2 * size, -1);
		gridBorders.c = gridTo2DCoords(2 * size, size - 1);
		break;
	default:
		std::cout << "not a valid loc index" << std::endl;
		break;
	}

	glm::vec2 coords = gridTo2DCoords(x, y);
	return barycentric(gridBorders.a, gridBorders.b, gridBorders.c, coords);
}

glm::vec3 Planet::barycentric(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 p) const
{
	glm::vec2 v0 = b - a, v1 = c - a, v2 = p - a;
	float d00 = glm::dot(v0, v0);
	float d01 = glm::dot(v0, v1);
	float d11 = glm::dot(v1, v1);
	float d20 = glm::dot(v2, v0);
	float d21 = glm::dot(v2, v1);
	float denom = d00 * d11 - d01 * d01;
	float v = (d11 * d20 - d01 * d21) / denom;
	float w = (d00 * d21 - d01 * d20) / denom;
	float u = 1.0f - v - w;

	return glm::vec3(u, v, w);
}

void Planet::renderHex(Voxel v) const
{
	GLuint colorIndex;
	switch ((*grid[v.grid])(v.x, v.y, v.z)) {
	case BlockType::None:
		return;
	case BlockType::red:
		colorIndex = 0;
		break;
	case BlockType::green:
		colorIndex = 1;
		break;
	case BlockType::black:
		colorIndex = 2;
		break;
	default:
		std::cout << "invalid block Type" << std::endl;
		return;
	}


	ChunkLoc c = GetChunkLoc(v);
	int chunkIndex = c.index * 5 + c.chunckRoot;

	Vertex vertices[12];

	// bottom vertices
	for (int i = 0; i < 6; i++) {
		vertices[i].colorIndex = colorIndex;
		Voxel neighbor1 = this->getNeighbor(v, i);
		Voxel neighbor2 = this->getNeighbor(v, (i + 1) % 6);
		vertices[i].position = (this->voxelTo3DCoords(v) + this->voxelTo3DCoords(neighbor1) + this->voxelTo3DCoords(neighbor2)) / 3.0f;
	}

	v.z = v.z + 1;
	// top vertices 
	for (int i = 0; i < 6; i++) {
		vertices[i + 6].colorIndex = colorIndex;
		Voxel neighbor1 = this->getNeighbor(v, i);
		Voxel neighbor2 = this->getNeighbor(v, (i + 1) % 6);
		vertices[i + 6].position = (this->voxelTo3DCoords(v) + this->voxelTo3DCoords(neighbor1) + this->voxelTo3DCoords(neighbor2)) / 3.0f;
	}

	// add vertices to vertexArray
	int lowerVertexIndices[4][3] = {
		{0,1,5}, {1,2,5}, {2,4,5}, {2,3,4},
	};

	for (int i = 0; i < 4; i++) {
		chunks[chunkIndex]->vertexArray.push_back(vertices[lowerVertexIndices[i][0]]);
		chunks[chunkIndex]->vertexArray.push_back(vertices[lowerVertexIndices[i][1]]);
		chunks[chunkIndex]->vertexArray.push_back(vertices[lowerVertexIndices[i][2]]);
	}

	for (int i = 0; i < 4; i++) {
		chunks[chunkIndex]->vertexArray.push_back(vertices[lowerVertexIndices[i][0] + 6]);
		chunks[chunkIndex]->vertexArray.push_back(vertices[lowerVertexIndices[i][1] + 6]);
		chunks[chunkIndex]->vertexArray.push_back(vertices[lowerVertexIndices[i][2] + 6]);
	}

	for (int i = 0; i < 6; i++) {
		// triangle 1 of side i
		chunks[chunkIndex]->vertexArray.push_back(vertices[i]);
		chunks[chunkIndex]->vertexArray.back().ambientOcclusion = 1;
		chunks[chunkIndex]->vertexArray.push_back(vertices[(i + 1) % 6]);
		chunks[chunkIndex]->vertexArray.back().ambientOcclusion = 1;
		chunks[chunkIndex]->vertexArray.push_back(vertices[i + 6]);
		chunks[chunkIndex]->vertexArray.back().ambientOcclusion = 1;
		// triangle 2 of side i
		chunks[chunkIndex]->vertexArray.push_back(vertices[(i + 1) % 6]);
		chunks[chunkIndex]->vertexArray.back().ambientOcclusion = 1;
		chunks[chunkIndex]->vertexArray.push_back(vertices[i + 6]);
		chunks[chunkIndex]->vertexArray.back().ambientOcclusion = 1;
		chunks[chunkIndex]->vertexArray.push_back(vertices[(i + 1) % 6 + 6]);
		chunks[chunkIndex]->vertexArray.back().ambientOcclusion = 1;
	}
}


void Planet::updateVertexArray(int chunk)
{
	assert(chunks[chunk]->update);
	chunks[chunk]->update = false;
	//Vertex v1, v2, v3;
	//v1.colorIndex = 0;
	//v2.colorIndex = 1;
	//v3.colorIndex = 2;

	//switch (loc.index)
	//{
	//case 0:
	//	v1.position = gridTo3Dcoords(0, -1, 2);
	//	v2.position = gridTo3Dcoords(0, size - 1, 2);
	//	v3.position = gridTo3Dcoords(size, -1, 2);
	//	break;
	//case 1:
	//	v1.position = gridTo3Dcoords(0, size - 1, 2);
	//	v2.position = gridTo3Dcoords(size, -1, 2);
	//	v3.position = gridTo3Dcoords(size, size - 1, 2);
	//	break;
	//case 2:
	//	v1.position = gridTo3Dcoords(size, -1, 2);
	//	v2.position = gridTo3Dcoords(size, size - 1, 2);
	//	v3.position = gridTo3Dcoords(2 * size, -1, 2);
	//	break;
	//case 3:
	//	v1.position = gridTo3Dcoords(size, size - 1, 2);
	//	v2.position = gridTo3Dcoords(2 * size, -1, 2);
	//	v3.position = gridTo3Dcoords(2 * size, size - 1, 2);
	//	break;
	//default:
	//	std::cout << "not a valid loc index" << std::endl;
	//	break;
	//}

	//vertexArray.push_back(v1);
	//vertexArray.push_back(v2);
	//vertexArray.push_back(v3);
	// update vertex array

	chunks[chunk]->vertexArray = std::vector<Vertex>();
	ChunkLoc loc = chunks[chunk]->getLoc();
	if (loc.index == 0) {
		for (int z = 0; z < maxHeight; z++) {
			for (int y = 0; y < size; y++) {
				for (int x = 0; x < size - y; x++) {
					//if (x %3 ==1) {
					//	(*grid)(x, y, 2) = BlockType::black;
					//}
					//if (x % 3 == 0) {
					//	(*grid)(x, y, 2) = BlockType::red;
					//}
					//if (x % 3 == 2) {
					//	(*grid)(x, y, 2) = BlockType::green;
					//}
					Voxel v = {loc.chunckRoot, x, y, z };
					renderHex(v);
				}
			}
		}
	}
	else if (loc.index == 1) {
		for (int z = 0; z < maxHeight; z++) {
			for (int y = 0; y < size; y++) {
				for (int x = size - y; x < size; x++) {
					Voxel v = { loc.chunckRoot, x, y, z };
					renderHex(v);
				}
			}
		}
	}
	else if (loc.index == 2) {
		for (int z = 0; z < maxHeight; z++) {
			for (int y = 0; y < size; y++) {
				for (int x = size; x < size * 2 - y; x++) {

					Voxel v = { loc.chunckRoot, x, y, z };
					renderHex(v);
				}
			}
		}
	}
	else if (loc.index == 3) {
		for (int z = 0; z < maxHeight; z++) {
			for (int y = 0; y < size; y++) {
				for (int x = 2 * size - y; x < size * 2; x++) {
					Voxel v = { loc.chunckRoot, x, y, z };
					renderHex(v);
				}
			}
		}
	}

	if (chunks[chunk]->vertexArray.empty()) {
		return;
	}
	glBindVertexArray(chunks[chunk]->getVAO());
	glBindBuffer(GL_ARRAY_BUFFER, chunks[chunk]->getVBO());

	glBufferData(GL_ARRAY_BUFFER, chunks[chunk]->getNumOfVertices() * sizeof(Vertex), &chunks[chunk]->vertexArray[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribIPointer(1, 1, GL_UNSIGNED_SHORT, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribIPointer(2, 1, GL_UNSIGNED_SHORT, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat) + sizeof(GLint)));
	glEnableVertexAttribArray(2);


	// unbind to make sure other code does not change it somewhere else
	glBindVertexArray(0);
}
