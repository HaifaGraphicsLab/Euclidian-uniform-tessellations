#pragma once
enum BlockType {
	None, 
	black,
	red,
	green
};


class Grid {
public:
	Grid(size_t x, size_t y, size_t z);
	BlockType& operator()(size_t x, size_t y, size_t z) const;
	size_t getX() const;
	size_t getY() const;
	size_t getZ() const;
	void print() const;
	~Grid();




private:
	BlockType* data;
	size_t x;
	size_t y;
	size_t z;

	// no assignment or copying
	Grid& operator=(const Grid&) = delete;
	Grid(Grid const&) = delete;
};