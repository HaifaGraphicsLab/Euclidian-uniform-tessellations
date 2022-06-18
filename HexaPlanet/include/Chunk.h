#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Grid.h"


struct Voxel {
	int grid;
	int x;
	int y;
	int z;
};

struct PixelInfo {
	unsigned int gridAndNeighbor = 6;
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int z = 0;
};

PixelInfo createPixelInfo(const Voxel& v, int neighbor);
Voxel extractPixelInfo(const PixelInfo& pi, bool* isSuccessful, int* neighbor);


struct Vertex
{
	glm::vec3 position;
	GLuint colorIndex;
	GLuint ambientOcclusion = 0;
	PixelInfo info;
};

struct GridBorder
{
	glm::vec2 a;
	glm::vec2 b;
	glm::vec2 c;
};

struct ChunkBorder
{
	glm::vec3 a;
	glm::vec3 b;
	glm::vec3 c;
};

struct ChunkLoc
{
	int chunckRoot;
	int index;
};

class Chunk {
public:
	Chunk(ChunkBorder borders, ChunkLoc loc, Grid* grid);
	ChunkBorder getBorders() const;
	GLuint getVAO();
	GLuint getPickerVAO();
	GLuint getVBO();
	GLuint getPickerVBO();
	GLsizei getNumOfVertices() const;
	ChunkLoc getLoc() const;
	bool update;
	std::vector<Vertex> vertexArray;

private:
	ChunkBorder borders;
	ChunkLoc loc;
	Grid* grid;

	int size;
	
	GLuint vbo;
	GLuint vao;
	GLuint pickerVao;
	GLuint pickerVbo;
};