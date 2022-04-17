#pragma once
#include <string>

enum BlockType {
	None, 
	black,
	red,
	green
};


class Grid {
public:
	Grid(int x, int y, int z, bool extra=false);
	BlockType& operator()(int x, int y, int z) const;
	BlockType& operator()(std::string s, int z) const;
	int getX() const;
	int getY() const;
	int getZ() const;
	void print() const;
	void print(std::string s) const;
	~Grid();




private:
	BlockType* data;
	BlockType* extra;
	bool hasExtra;
	int x;
	int y;
	int z;



	// no assignment or copying
	Grid& operator=(const Grid&) = delete;
	Grid(Grid const&) = delete;
};