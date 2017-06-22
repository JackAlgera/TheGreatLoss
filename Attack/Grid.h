#pragma once

#include <SFML\Graphics.hpp>
#include <cstdlib> 
#include <iostream>
#include "Cell.h"

/*
0 :: Neutral territory
1 :: player 1
2 :: player 2
*/
	

class Grid
{
public:
public:
	Grid(int nbrCells, int cellSize);
	~Grid();

	void draw(sf::RenderWindow& window);
	void update();
	void setCell(int i, int j, int player);
	void setMatriceCells(std::vector<int> matriceCells);
	int getCell(int i, int j);
	void updateMatriceCells();
	void restart(bool random, sf::RenderWindow& window);

	void testCells(int i, int j, int& nbr1, int& nbr2);

	void drawCell(int i, int j, sf::RenderWindow& window);

	void setMid(int i, int j); //0<i,j<99
	void setLeft(int i, int j); //0<i<99, j==0
	void setRight(int i, int j); //0<i<99, j==99
	void setTop(int i, int j);	//0<j<99, i==0
	void setBot(int i, int j);	//0<j<99, i==99
	void setTopLeft(int i, int j); //j==i==0
	void setTopRight(int i, int j);	//j==99,i==0
	void setBotRight(int i, int j);	//j==i==99
	void setBotLeft(int i, int j);	//j==0, i==99


private:

	std::vector<int> matriceCells;
	std::vector<int> tempMatriceCells;
	std::vector<Cell> changedCells;
	std::vector<Cell> tempChangedCells;
	int nbrCells;
	int cellSize;
};

