#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Grid.h"

struct Vertex
{
	glm::vec3 position;
	int colorIndex;
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
	GLuint getVAO();
	GLsizei getNumOfVertices() const;


private:
	ChunkBorder borders;
	GridBorder gridBorders;
	ChunkLoc loc;
	Grid* grid;

	int size;
	float normalizedHexRadius;
	glm::vec2 normalizedHexVectors[6];
	void updateVertexArray();
	glm::vec3 gridTo3Dcoords(int x, int y, int z) const;
	std::vector<Vertex> vertexArray;
	bool update;
	
	glm::vec2 gridTo2DCoords(int x, int y) const;
	glm::vec3 gridToBarycentric(int x, int y) const;
	glm::vec3 barycentric(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 p) const;
	glm::vec3 slerp(glm::vec3 p0, glm::vec3 p1, float t) const;
	void renderHex(int x, int y, int z);
	float heightFunc(int z) const;

	GLuint vbo;
	GLuint vao;
};