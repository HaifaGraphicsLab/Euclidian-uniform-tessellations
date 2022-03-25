#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertix
{
	glm::vec3 position;
	int colorIndex;
};


class Chunk {
public:
	Chunk(glm::vec3 border_a, glm::vec3 border_b, glm::vec3 border_c);
	GLuint getVAO();
	GLsizei getNumOfVertices() const;


private:
	glm::vec3 borders[3];

	void updateVertexArray();
	std::vector<Vertix> vertexArray;
	bool update;

	GLuint vbo;
	GLuint vao;
};