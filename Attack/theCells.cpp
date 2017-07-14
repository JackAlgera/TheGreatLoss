#include "theCells.h"
#include <Windows.h>
#include <ctime>
#include <cstdlib> 
#include <iostream>

theCells::theCells(int nbrCells, int cellSize)			//Initializes each cell with 0, thus clearing the field
{
	this->nbrCells = nbrCells;
	this->cellSize = cellSize;
	this->matriceCells.resize(nbrCells*nbrCells);
	this->temp.resize(nbrCells*nbrCells);
}

theCells::~theCells()
{
}

void theCells::draw(sf::RenderWindow& window)					//Draws the squares, using the matrix to determine the color of each square
{
	for (int i = 0; i < nbrCells; i++)
	{
		for (int j = 0; j < nbrCells; j++)
		{
			this->drawCell(i, j,window);
		}
	}
}

void theCells::update()										//Updates each square, being careful to not do the same tests on each square as not all squares have the same amount of neighbors (e.g square in the middle has 8 neighbors and a square in a corner only has 3)
{
	for (int i = 0; i < nbrCells; i++)
	{
		for (int j = 0; j < nbrCells; j++)
		{
			if ((i < nbrCells-1) && (j < nbrCells-1) && (i>0) && (j>0))
			{
				this->setMid(i, j);
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
	}
	this->updateMatriceCells();
}

void theCells::setCell(int i, int j, int player)			
{
	temp[j + i*nbrCells] = player;
}

void theCells::setMatriceCells(int newMatriceCells[])			//Used in the function that updates the matrix, I use a temp matrix to note each cell's changes, as to not update a cell before finishing all the tests on every cell.
{
	for (int i = 0; i < nbrCells; i++)
	{
		for (int j = 0; j < nbrCells; j++)
		{
			this->matriceCells[j + i * nbrCells] = newMatriceCells[j + i * nbrCells];
		}
	}
}

int theCells::getCell(int i, int j)
{
	return matriceCells[j + i*nbrCells];
}

void theCells::updateMatriceCells()								//Copies the temp matrix to the actual matrix
{
	for (int i = 0; i < nbrCells; i++)
	{
		for (int j = 0; j < nbrCells; j++)
		{
			this->matriceCells[j + i * nbrCells] = this->temp[j + i * nbrCells];
		}
	}
}

void theCells::restart(bool random, sf::RenderWindow& window)				//Reinitializes the program, being able to pick a random starting position
{
	for (int i = 0; i < nbrCells; i++)
	{
		for (int j = 0; j < nbrCells; j++)
		{
			this->matriceCells[j + i * nbrCells] = 0;
			this->temp[j + i * nbrCells] = 0;
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


void theCells::testCells(int i, int j, int& nbr1, int& nbr2)			//Used in the functions that update each cell for a given position (e.g the middle cell having 8 neighbors, does not have the same function to determine it's new color compared to a cell from a corner, which only has 3 neighbors).
{
	int cellNumber = matriceCells.at(j + i * nbrCells);
	if (cellNumber == 1)
		nbr1++;
	else if (cellNumber == 2)
		nbr2++;
}

void theCells::drawCell(int i, int j, sf::RenderWindow& window)				//Draws a given square or cell
{
	sf::VertexArray cell(sf::PrimitiveType::Quads, 4);
	sf::Color color;

	cell[0].position = sf::Vector2f((j + 1) + j*cellSize, (i + 1) + i*cellSize);	//Used to draw a vertex, and thus a cell
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

void theCells::setMid(int i, int j)					//For : 0<i , j<99 (e.g the middle 1x1 squares of a 100x100 square, the top left corner square being i=0 and j=0)
{													//The function counts the amount of neighbors (and it's color) of a given color there are to the given square and then determines what color it will change to, the more of a given color there is, the more likely the cell will change to that color
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

void theCells::setLeft(int i, int j)			//0<i<99, j==0
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

void theCells::setRight(int i, int j)			//0<i<99, j==99
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

void theCells::setTop(int i, int j)				//0<j<99, i==0
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

void theCells::setBot(int i, int j)				//0<j<99, i==99
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

void theCells::setTopLeft(int i, int j)			//j==i==0
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

void theCells::setTopRight(int i, int j)		//j==99,i==0
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

void theCells::setBotRight(int i, int j)		//j==i==99
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

void theCells::setBotLeft(int i, int j)			//j==0, i==99
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
