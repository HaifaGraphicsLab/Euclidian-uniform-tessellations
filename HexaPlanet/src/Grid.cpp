#include "Grid.h"
#include <iostream>
#include <cassert>

#define assertm(exp, msg) assert(((void)msg, exp))




Grid::Grid(int x, int y, int z) : x(x), y(y), z(z){
	this->data = new BlockType[x * y * z];
	for (int i = 0; i < x * y * z; i++) {
		this->data[i] = BlockType::None;
	}
}

BlockType& Grid::operator()(int x, int y, int z) const{
	assertm((x < this->x) && x >= 0, "X out of bounds");
	assertm((y < this->y) && y >= 0, "Y out of bounds");
	assertm((z < this->z) && z >= 0, "Z out of bounds");

	int loc = z * this->x * this->y + y * this->x + x;
	return data[loc];
}

int Grid::getX() const{
	return x;
}
int Grid::getY() const {
	return y;
}
int Grid::getZ() const {
	return z;
}
void Grid::print() const{
	for (int i = 0; i < z; i++) {
		for (int j = 0; j < y; j++) {
			for (int k = 0; k < x; k++) {
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