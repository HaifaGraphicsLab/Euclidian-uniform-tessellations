#include "Grid.h"
#include <iostream>
#include <cassert>

#define assertm(exp, msg) assert(((void)msg, exp))




Grid::Grid(size_t x, size_t y, size_t z) : x(x), y(y), z(z){
	this->data = new BlockType[x * y * z];
	for (int i = 0; i < x * y * z; i++) {
		this->data[i] = BlockType::None;
	}
}

BlockType& Grid::operator()(size_t x, size_t y, size_t z) const{
	assertm(x < this->x && x > 0, "X out of bounds");
	assertm(y < this->y && y > 0, "Y out of bounds");
	assertm(z < this->z && z > 0, "Z out of bounds");

	size_t loc = z * this->x * this->y + y * this->x + this->x;
	return data[loc];
}

size_t Grid::getX() const{
	return x;
}
size_t Grid::getY() const {
	return y;
}
size_t Grid::getZ() const {
	return z;
}
void Grid::print() const{
	for (int i = 0; i < z; i++) {
		for (int j = 0; j < y; j++) {
			for (int k = 0; k < z; k++) {
				std::cout << operator()(k, j, i) << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl << std::endl;
	}
}

Grid::~Grid() {
	delete[] data;
}