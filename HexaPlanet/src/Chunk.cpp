#include "Chunk.h"
#include <iostream>


Chunk::Chunk(ChunkBorder borders, ChunkLoc loc, Grid* grid)
{
	this->borders = borders;
	this->loc = loc;
	this->grid = grid;
	this->size = grid->getY(); // size is as the grid height


	// decide grid borders
	switch (loc.index)
	{
	case 0:
		this->gridBorders.a = gridTo2DCoords(0, 0);
		this->gridBorders.b = gridTo2DCoords(0, size);
		this->gridBorders.c = gridTo2DCoords(size, 0);
		break;
	case 1:
		this->gridBorders.a = gridTo2DCoords(size, 0);
		this->gridBorders.b = gridTo2DCoords(0, size);
		this->gridBorders.c = gridTo2DCoords(size, size);
		break;
	case 2:
		this->gridBorders.a = gridTo2DCoords(size, 0); 
		this->gridBorders.b = gridTo2DCoords(size, size);
		this->gridBorders.c = gridTo2DCoords(2*size, 0);
		break;
	case 3:
		this->gridBorders.a = gridTo2DCoords(size, size);
		this->gridBorders.b = gridTo2DCoords(2*size, 0);
		this->gridBorders.c = gridTo2DCoords(2 * size, size);
		break;
	default:
		std::cout << "not a valid loc index" << std::endl;
		break;
	}

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
	vertexArray = std::vector<Vertex>();
	if (loc.index == 0) {
		for (int z = 0; z < grid->getZ(); z++) {
			for (int y = 0; y < size; y++) {
				for (int x = 0; x < size - y; x++) {
					if (z == 2) {
						if ((x) % 3 == 0) {
							(*grid)(x, y, z) = BlockType::red;
						}
						if ((x) % 3 == 1) {
							(*grid)(x, y, z) = BlockType::black;
						}
						if ((x) % 3 == 2) {
							(*grid)(x, y, z) = BlockType::green;
						}
					}
					renderHex(x,y,z);
				}
			}
		}
	}else if (loc.index == 1) {
		for (int z = 0; z < grid->getZ(); z++) {
			for (int y = 0; y < size; y++) {
				for (int x = size-y; x < size; x++) {
					if (z == 2) {
						if ((x) % 3 == 0) {
							(*grid)(x, y, z) = BlockType::red;
						}
						if ((x) % 3 == 1) {
							(*grid)(x, y, z) = BlockType::black;
						}
						if ((x) % 3 == 2) {
							(*grid)(x, y, z) = BlockType::green;
						}
					}
					renderHex(x, y, z);
				}
			}
		}
	}else if (loc.index == 2) {
		for (int z = 0; z < grid->getZ(); z++) {
			for (int y = 0; y < size; y++) {
				for (int x = size; x < size*2 - y; x++) {
					if ((x) % 3 == 0) {
						(*grid)(x, y, z) = BlockType::red;
					}
					if ((x) % 3 == 1) {
						(*grid)(x, y, z) = BlockType::black;
					}
					if ((x) % 3 == 2) {
						(*grid)(x, y, z) = BlockType::green;
					}
					renderHex(x, y, z);
				}
			}
		}
	}else if (loc.index == 3) {
		for (int z = 0; z < grid->getZ(); z++) {
			for (int y = 0; y < size; y++) {
				for (int x = 2*size - y; x < size * 2; x++) {
					if (z == 2) {
						if ((x + y) % 3 == 0) {
							(*grid)(x, y, z) = BlockType::red;
						}
						if ((x + y) % 3 == 1) {
							(*grid)(x, y, z) = BlockType::black;
						}
						if ((x + y) % 3 == 2) {
							(*grid)(x, y, z) = BlockType::green;
						}
					}
					renderHex(x, y, z);
				}
			}
		}
	}
	//if (vertexArray.empty()) {
	//	vertexArray.push_back({glm::vec3(), 0});
	//}

	Vertex v1;
	v1.position = borders.a;
	v1.colorIndex = loc.chunckRoot;
	vertexArray.push_back(v1);
	Vertex v2;
	v2.position = borders.b;
	v2.colorIndex = loc.chunckRoot;
	vertexArray.push_back(v2);
	Vertex v3;
	v3.position = borders.c;
	v3.colorIndex = loc.chunckRoot;
	vertexArray.push_back(v3);
	
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof(Vertex), &vertexArray[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribIPointer(1, 1, GL_INT, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);


	// unbind to make sure other code does not change it somewhere else
	glBindVertexArray(0);
}

glm::vec3 Chunk::gridTo3Dcoords(int x, int y, int z) const
{
	// get barycentiric coordinates
	glm::vec3 barycentric = gridToBarycentric(x, y);

	// perform double slerp
	float l2s = barycentric.x / (barycentric.x + barycentric.y);
	glm::vec3 p12 = slerp(borders.a, borders.b, l2s);
	return slerp(p12, borders.c, barycentric.z)*heightFunc(z);
}


glm::vec2 Chunk::gridTo2DCoords(int x, int y) const
{
	float x_coord = (sqrt(3) * x + sqrt(3) / 2 * y);
	float y_coord = 3.0 / 2 * y;

	return glm::vec2(x_coord, y_coord);
}

glm::vec3 Chunk::gridToBarycentric(int x, int y) const
{
	glm::vec2 coords = gridTo2DCoords(x, y);
	return barycentric(gridBorders.a, gridBorders.b, gridBorders.c, coords);
}

glm::vec3 Chunk::barycentric(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 p) const
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

glm::vec3 Chunk::slerp(glm::vec3 p0, glm::vec3 p1, float t) const{
	float cos = glm::dot(p0, p1) / glm::dot(p0, p0);
	float sin = sqrt(1.0 - cos*cos);
	float alpha = atan2(sin, cos);
	float l0 = std::sin((1-t)*alpha);
	float l1 = std::sin(t * alpha);
	return (l0 * p0 + l1 * p1) / sin;
}

void Chunk::renderHex(int x, int y, int z)
{
	GLuint colorIndex;
	switch ((*grid)(x, y, z)) {
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

	glm::vec3 bottomCenter = gridTo3Dcoords(x, y, z);
	glm::vec3 topCenter = gridTo3Dcoords(x, y, z+1);
	Vertex vertices[12];
	int neighbors[6][2] = { {0, -1}, {1, -1}, {1, 0}, {0, 1}, {-1, 1}, {-1, 0} };
	
	// bottom vertices
	for (int i = 0; i < 6; i++) {
		vertices[i].colorIndex = colorIndex;
		glm::vec3 neighbor1 = gridTo3Dcoords(x + neighbors[i][0], y + neighbors[i][1], z);
		glm::vec3 neighbor2 = gridTo3Dcoords(x + neighbors[(i+1)%6][0], y + neighbors[(i + 1) % 6][1], z);
		vertices[i].position = (bottomCenter + neighbor1 + neighbor2) / 3.0f;

	}
	
	// top vertices 
	for (int i = 0; i < 6; i++) {
		vertices[i+6].colorIndex = colorIndex;
		glm::vec3 neighbor1 = gridTo3Dcoords(x + neighbors[i][0], y + neighbors[i][1], z+1);
		glm::vec3 neighbor2 = gridTo3Dcoords(x + neighbors[(i + 1) % 6][0], y + neighbors[(i + 1) % 6][1], z+1);
		vertices[i+6].position = (topCenter + neighbor1 + neighbor2) / 3.0f;
	}

	// add vertices to vertexArray
	int lowerVertexIndices[4][3] = {
		{0,1,5}, {1,2,5}, {2,4,5}, {2,3,4},
	};

	for (int i = 0; i < 4; i++) {
		vertexArray.push_back(vertices[lowerVertexIndices[i][0]]);
		vertexArray.push_back(vertices[lowerVertexIndices[i][1]]);
		vertexArray.push_back(vertices[lowerVertexIndices[i][2]]);
	}

	for (int i = 0; i < 4; i++) {
		vertexArray.push_back(vertices[lowerVertexIndices[i][0]+6]);
		vertexArray.push_back(vertices[lowerVertexIndices[i][1]+6]);
		vertexArray.push_back(vertices[lowerVertexIndices[i][2]+6]);
	}

	for (int i = 0; i < 6; i++) {
		// triangle 1 of side i
		vertexArray.push_back(vertices[i]);
		vertexArray.push_back(vertices[(i + 1) % 6]);
		vertexArray.push_back(vertices[i + 6]);
		// triangle 2 of side i
		vertexArray.push_back(vertices[(i + 1) % 6]);
		vertexArray.push_back(vertices[i + 6]);
		vertexArray.push_back(vertices[(i + 1) % 6 + 6]);
	}

}



float Chunk::heightFunc(int z) const
{
	return 5+z/size;
}
