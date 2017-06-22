#pragma once
class Cell
{
public:
	Cell(int i, int j);
	~Cell();

	int getPosI() { return i; }
	int getPosJ() { return j; }

private:
	int i, j;
};

