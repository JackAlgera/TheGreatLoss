#include "Grid.h"
#include <Windows.h>
#include <ctime>
#include <cstdlib> 
#include <iostream>

Grid::Grid(int nbrCells, int cellSize)
{
	this->nbrCells = nbrCells;
	this->cellSize = cellSize;
	this->matriceCells.resize(nbrCells*nbrCells);
	this->tempMatriceCells.resize(nbrCells*nbrCells);
}

Grid::~Grid()
{
}

void Grid::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < nbrCells; i++)
	{
		for (int j = 0; j < nbrCells; j++)
		{
			this->drawCell(i, j,window);
		}
	}
}

void Grid::update()
{
	changedCells.clear();
	for (Cell cell : tempChangedCells)
	{
		changedCells.push_back(cell);
	}
	tempChangedCells.clear();
	for(Cell cell : changedCells)
	{
		int i = cell.getPosI();
		int j = cell.getPosJ();
		if ((i < nbrCells-2) && (j < nbrCells-2) && (i>1) && (j>1))
		{
			this->setMid(i - 1, j - 1);
			this->setMid(i - 1, j);
			this->setMid(i - 1, j + 1);
			this->setMid(i, j - 1);
			this->setMid(i, j);
			this->setMid(i, j + 1);
			this->setMid(i + 1, j - 1);
			this->setMid(i + 1, j);
			this->setMid(i + 1, j + 1);
		}
		else if ((i < nbrCells - 1) && (i>0) && (j==0))
		{
			this->setLeft(i, j);
		}
		else if ((i < nbrCells - 1) && (i>0) && (j == nbrCells-1))
		{
			this->setRight(i, j);
		}
		else if ((j < nbrCells - 1) && (j>0) && (i == 0))
		{
			this->setTop(i, j);
		}
		else if ((j < nbrCells - 1) && (j>0) && (i == nbrCells - 1))
		{
			this->setBot(i, j);
		}
		else if ((i == 0) && (j == 0))
		{
			this->setTopLeft(i, j);
		}
		else if ((i == 0) && (j == nbrCells - 1))
		{
			this->setTopRight(i, j);
		}
		else if ((i == nbrCells - 1) && (j == nbrCells - 1))
		{
			this->setBotRight(i, j);
		}
		else if ((i == nbrCells - 1) && (j == 0))
		{
			this->setBotLeft(i, j);
			
		}
	}
	this->updateMatriceCells();
}

void Grid::setCell(int i, int j, int player)
{
	tempMatriceCells.at(j + i*nbrCells) = player;
	tempChangedCells.push_back(Cell(i, j));
}

void Grid::setMatriceCells(std::vector<int> newMatriceCells)
{
	for (int i = 0; i < nbrCells; i++)
	{
		for (int j = 0; j < nbrCells; j++)
		{
			this->matriceCells.at(j + i*nbrCells) = newMatriceCells.at(j + i*nbrCells);
		}
	}
}

int Grid::getCell(int i, int j)
{
	return matriceCells.at(j + i*nbrCells);
}

void Grid::updateMatriceCells()
{
	for (int i = 0; i < nbrCells; i++)
	{
		for (int j = 0; j < nbrCells; j++)
		{
			this->matriceCells.at(j + i*nbrCells) = this->tempMatriceCells.at(j + i*nbrCells);
		}
	}
}

void Grid::restart(bool random, sf::RenderWindow& window)
{
	for (int i = 0; i < nbrCells; i++)
	{
		for (int j = 0; j < nbrCells; j++)
		{
			this->matriceCells.at(j + i*nbrCells) = 0;
			this->tempMatriceCells.at(j + i*nbrCells) = 0;
		}
	}
	if (!random)
	{
		this->setCell(0, 0, 2);
		this->setCell(nbrCells - 1, nbrCells - 1, 1);
	}
	else
	{
		this->setCell(rand() % nbrCells, rand() % nbrCells, 2);
		this->setCell(rand() % nbrCells, rand() % nbrCells, 1);
	}
	this->updateMatriceCells();
	this->draw(window);
}


void Grid::testCells(int i, int j, int& nbr1, int& nbr2)
{
	int cellNumber = matriceCells.at(j + i * nbrCells);
	if (cellNumber == 1)
		nbr1++;
	else if (cellNumber == 2)
		nbr2++;
}

void Grid::drawCell(int i, int j, sf::RenderWindow& window)
{
	sf::VertexArray cell(sf::PrimitiveType::Quads, 4);
	sf::Color color;

	cell[0].position = sf::Vector2f((j + 1) + j*cellSize, (i + 1) + i*cellSize);	//Specifying the 4 corners of the cell
	cell[1].position = sf::Vector2f((j + 1) + j*cellSize + cellSize, (i + 1) + i*cellSize);
	cell[2].position = sf::Vector2f((j + 1) + j*cellSize + cellSize, (i + 1) + i*cellSize + cellSize);
	cell[3].position = sf::Vector2f((j + 1) + j*cellSize, (i + 1) + i*cellSize + cellSize);


	if (matriceCells.at(j + i * nbrCells) == 0)
		color = sf::Color::White;
	else if (matriceCells.at(j + i * nbrCells) == 1)
		color = sf::Color::Black;
	else if (matriceCells.at(j + i * nbrCells) == 2)
		color = sf::Color::Red;


	cell[0].color = color;
	cell[1].color = color;
	cell[2].color = color;
	cell[3].color = color;

	window.draw(cell);
}

void Grid::setMid(int i, int j)
{
	double randNum = (double)rand();
	double chance = randNum / (double)RAND_MAX;

	int nbr1 = 0;
	int nbr2 = 0;
	this->testCells(i-1,j-1, nbr1, nbr2);
	this->testCells(i-1,j, nbr1, nbr2);
	this->testCells(i-1,j+1, nbr1, nbr2);
	this->testCells(i,j-1, nbr1, nbr2);
	this->testCells(i, j, nbr1, nbr2);
	this->testCells(i,j+1, nbr1, nbr2);
	this->testCells(i+1,j-1, nbr1, nbr2);
	this->testCells(i+1,j, nbr1, nbr2);
	this->testCells(i+1,j+1, nbr1, nbr2);

	if (chance < (double)nbr1/9.0)
		this->setCell(i, j, 1);
	else if (chance > (1.0-(double)nbr2/9.7))
		this->setCell(i, j, 2);

}

void Grid::setLeft(int i, int j)
{
	double randNum = (double)rand();
	double chance = randNum / (double)RAND_MAX;

	int nbr1 = 0;
	int nbr2 = 0;
	this->testCells(i - 1, j, nbr1, nbr2);
	this->testCells(i - 1, j + 1, nbr1, nbr2);
	this->testCells(i, j + 1, nbr1, nbr2);
	this->testCells(i + 1, j + 1, nbr1, nbr2);
	this->testCells(i + 1, j, nbr1, nbr2);

	if (chance < (double)nbr1 / 5.0)
		this->setCell(i, j, 1);
	else if (chance >(1.0 - (double)nbr2 / 5.0))
		this->setCell(i, j, 2);

}

void Grid::setRight(int i, int j)
{
	double randNum = (double)rand();
	double chance = randNum / (double)RAND_MAX;

	int nbr1 = 0;
	int nbr2 = 0;
	this->testCells(i - 1, j, nbr1, nbr2);
	this->testCells(i - 1, j - 1, nbr1, nbr2);
	this->testCells(i, j - 1, nbr1, nbr2);
	this->testCells(i + 1, j - 1, nbr1, nbr2);
	this->testCells(i + 1, j, nbr1, nbr2);

	if (chance < (double)nbr1 / 5.0)
		this->setCell(i, j, 1);
	else if (chance >(1.0 - (double)nbr2 / 5.0))
		this->setCell(i, j, 2);
}

void Grid::setTop(int i, int j)
{
	double randNum = (double)rand();
	double chance = randNum / (double)RAND_MAX;

	int nbr1 = 0;
	int nbr2 = 0;
	this->testCells(i, j - 1, nbr1, nbr2);
	this->testCells(i + 1, j - 1, nbr1, nbr2);
	this->testCells(i + 1, j, nbr1, nbr2);
	this->testCells(i + 1, j + 1, nbr1, nbr2);
	this->testCells(i, j + 1, nbr1, nbr2);

	if (chance < (double)nbr1 / 5.0)
		this->setCell(i, j, 1);
	else if (chance >(1.0 - (double)nbr2 / 5.0))
		this->setCell(i, j, 2);
}

void Grid::setBot(int i, int j)
{
	double randNum = (double)rand();
	double chance = randNum / (double)RAND_MAX;

	int nbr1 = 0;
	int nbr2 = 0;
	this->testCells(i, j - 1, nbr1, nbr2);
	this->testCells(i - 1, j - 1, nbr1, nbr2);
	this->testCells(i - 1, j, nbr1, nbr2);
	this->testCells(i - 1, j + 1, nbr1, nbr2);
	this->testCells(i, j + 1, nbr1, nbr2);

	if (chance < (double)nbr1 / 5.0)
		this->setCell(i, j, 1);
	else if (chance >(1.0 - (double)nbr2 / 5.0))
		this->setCell(i, j, 2);
}

void Grid::setTopLeft(int i, int j)
{
	double randNum = (double)rand();
	double chance = randNum / (double)RAND_MAX;

	int nbr1 = 0;
	int nbr2 = 0;
	this->testCells(i + 1, j, nbr1, nbr2);
	this->testCells(i + 1, j + 1, nbr1, nbr2);
	this->testCells(i, j + 1, nbr1, nbr2);

	if (chance < (double)nbr1 / 3.0)
		this->setCell(i, j, 1);
	else if (chance >(1.0 - (double)nbr2 / 3.0))
		this->setCell(i, j, 2);
}

void Grid::setTopRight(int i, int j)
{
	double randNum = (double)rand();
	double chance = randNum / (double)RAND_MAX;

	int nbr1 = 0;
	int nbr2 = 0;
	this->testCells(i, j - 1, nbr1, nbr2);
	this->testCells(i + 1, j - 1, nbr1, nbr2);
	this->testCells(i + 1, j, nbr1, nbr2);

	if (chance < (double)nbr1 / 3.0)
		this->setCell(i, j, 1);
	else if (chance >(1.0 - (double)nbr2 / 3.0))
		this->setCell(i, j, 2);
}

void Grid::setBotRight(int i, int j)
{
	double randNum = (double)rand();
	double chance = randNum / (double)RAND_MAX;

	int nbr1 = 0;
	int nbr2 = 0;
	this->testCells(i - 1, j, nbr1, nbr2);
	this->testCells(i - 1, j - 1, nbr1, nbr2);
	this->testCells(i, j - 1, nbr1, nbr2);

	if (chance < (double)nbr1 / 3.0)
		this->setCell(i, j, 1);
	else if (chance >(1.0 - (double)nbr2 / 3.0))
		this->setCell(i, j, 2);
}

void Grid::setBotLeft(int i, int j)
{
	double randNum = (double)rand();
	double chance = randNum / (double)RAND_MAX;

	int nbr1 = 0;
	int nbr2 = 0;
	this->testCells(i, j + 1, nbr1, nbr2);
	this->testCells(i - 1, j + 1, nbr1, nbr2);
	this->testCells(i - 1, j, nbr1, nbr2);

	if (chance < (double)nbr1 / 3.0)
		this->setCell(i, j, 1);
	else if (chance >(1.0 - (double)nbr2 / 3.0))
		this->setCell(i, j, 2);
}
