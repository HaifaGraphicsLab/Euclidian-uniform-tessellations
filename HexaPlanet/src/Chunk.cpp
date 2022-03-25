#include "Chunk.h"
#include <iostream>

Chunk::Chunk(glm::vec3 border_a, glm::vec3 border_b, glm::vec3 border_c) : update(true)
{
	this->borders[0] = border_a;
	this->borders[1] = border_b;
	this->borders[2] = border_c;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	this->updateVertexArray();
}

GLuint Chunk::getVAO() 
{
	if (update) {
		this->updateVertexArray();
	}
	return vao;
}

GLsizei Chunk::getNumOfVertices() const
{
	return vertexArray.size();
}

void Chunk::updateVertexArray()
{
	assert(update);
	update = false;


	// update vertex array
	vertexArray = std::vector<Vertix>();
	int i = 0;
	for (glm::vec3 b : borders) {
		Vertix v;
		v.position = b;
		v.colorIndex = i%3;
		vertexArray.push_back(v);
		i++;
	}

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof(Vertix), &vertexArray[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertix), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribIPointer(1, 1, GL_INT, sizeof(Vertix), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);


	// unbind to make sure other code does not change it somewhere else
	glBindVertexArray(0);
}
