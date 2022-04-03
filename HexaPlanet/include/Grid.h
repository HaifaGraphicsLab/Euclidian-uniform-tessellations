#pragma once
enum BlockType {
	None, 
	black,
	red,
	green
};


class Grid {
public:
	Grid(int x, int y, int z);
	BlockType& operator()(int x, int y, int z) const;
	int getX() const;
	int getY() const;
	int getZ() const;
	void print() const;
	~Grid();




private:
	BlockType* data;
	int x;
	int y;
	int z;

	// no assignment or copying
	Grid& operator=(const Grid&) = delete;
	Grid(Grid const&) = delete;
};