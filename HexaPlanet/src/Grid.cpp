#include <iostream>
#include <cassert>
#include "Grid.h"



#define assertm(exp, msg) assert(((void)msg, exp))




Grid::Grid(int x, int y, int z, bool extra) : x(x), y(y), z(z){
	this->data = new BlockType[x * y * z];
	this->hasExtra = extra;
	for (int i = 0; i < x * y * z; i++) {
		this->data[i] = BlockType::None;
	}
	if (hasExtra) {
		this->extra = new BlockType[z];
		for (int i = 0; i < z; i++) {
			this->extra[i] = BlockType::None;
		}
	}
}

BlockType& Grid::operator()(int x, int y, int z) const{
	assertm((x < this->x) && x >= 0, "X out of bounds");
	assertm((y < this->y) && y >= 0, "Y out of bounds");
	assertm((z < this->z) && z >= 0, "Z out of bounds");

	int loc = z * this->x * this->y + y * this->x + x;
	return data[loc];
}

BlockType& Grid::operator()(std::string s, int z) const {
	assertm((z < this->z) && z >= 0, "Z out of bounds");
	
	for (auto& c : s) c = toupper(c);
	if (s == "EXTRA") {
		assertm(hasExtra, "grid has no extra");
		return extra[z];
	}
	else {
		assertm(false, "unknown string");
	}
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

void Grid::print(std::string s) const
{
	for (int i = 0; i < z; i++)
		std::cout << operator()(s, i) << " ";
	std::cout << std::endl;
}




Grid::~Grid() {
	delete[] data;
}