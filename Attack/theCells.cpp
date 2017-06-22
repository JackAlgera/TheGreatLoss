#include "theCells.h"
#include <Windows.h>
#include <ctime>
#include <cstdlib> 
#include <iostream>

theCells::theCells(int nbrCells, int cellSize)
{
	this->nbrCells = nbrCells;
	this->cellSize = cellSize;
	for (int i = 0; i < nbrCells; i++)
	{
		for (int j = 0; j < nbrCells; j++)
		{
			this->matriceCells[j + i * 100] = 0;
			this->temp[j + i * 100] = 0;
		}
	}
}

theCells::~theCells()
{
}

void theCells::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < nbrCells; i++)
	{
		for (int j = 0; j < nbrCells; j++)
		{
			this->drawCell(i, j,window);
		}
	}
}

void theCells::update()
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
			else if ((i < nbrCells - 1) && (i>0) && (j == 99))
			{
				this->setRight(i, j);
			}
			else if ((j < nbrCells - 1) && (j>0) && (i == 0))
			{
				this->setTop(i, j);
			}
			else if ((j < nbrCells - 1) && (j>0) && (i == 99))
			{
				this->setBot(i, j);
			}
			else if ((i == 0) && (j == 0))
			{
				this->setTopLeft(i, j);
			}
			else if ((i == 0) && (j == 99))
			{
				this->setTopRight(i, j);
			}
			else if ((i == 99) && (j == 99))
			{
				this->setBotRight(i, j);
			}
			else if ((i == 99) && (j == 0))
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

void theCells::setMatriceCells(int newMatriceCells[])
{
	for (int i = 0; i < nbrCells; i++)
	{
		for (int j = 0; j < nbrCells; j++)
		{
			this->matriceCells[j + i * 100] = newMatriceCells[j + i * 100];
		}
	}
}

int theCells::getCell(int i, int j)
{
	return matriceCells[j + i*nbrCells];
}

void theCells::updateMatriceCells()
{
	for (int i = 0; i < nbrCells; i++)
	{
		for (int j = 0; j < nbrCells; j++)
		{
			this->matriceCells[j + i * 100] = this->temp[j + i * 100];
		}
	}
}

void theCells::restart(bool random, sf::RenderWindow& window)
{
	for (int i = 0; i < nbrCells; i++)
	{
		for (int j = 0; j < nbrCells; j++)
		{
			this->matriceCells[j + i * 100] = 0;
			this->temp[j + i * 100] = 0;
		}
	}
	if (!random)
	{
		this->setCell(0, 0, 2);
		this->setCell(99, 99, 1);
	}
	else
	{
		this->setCell(rand() % 100, rand() % 100, 2);
		this->setCell(rand() % 100, rand() % 100, 1);
	}
	this->updateMatriceCells();
	this->draw(window);
}


void theCells::testCells(int i, int j, int& nbr1, int& nbr2)
{
	int cellNumber = matriceCells[j + i * 100];
	if (cellNumber == 1)
		nbr1++;
	else if (cellNumber == 2)
		nbr2++;
}

void theCells::drawCell(int i, int j, sf::RenderWindow& window)
{
	sf::VertexArray cell(sf::PrimitiveType::Quads, 4);
	sf::Color color;

	cell[0].position = sf::Vector2f((j + 1) + j*cellSize, (i + 1) + i*cellSize);	//Specifying the 4 corners of the cell
	cell[1].position = sf::Vector2f((j + 1) + j*cellSize + cellSize, (i + 1) + i*cellSize);
	cell[2].position = sf::Vector2f((j + 1) + j*cellSize + cellSize, (i + 1) + i*cellSize + cellSize);
	cell[3].position = sf::Vector2f((j + 1) + j*cellSize, (i + 1) + i*cellSize + cellSize);


	if (matriceCells[j + i*nbrCells] == 0)
		color = sf::Color::White;
	else if (matriceCells[j + i*nbrCells] == 1)
		color = sf::Color::Black;
	else if (matriceCells[j + i*nbrCells] == 2)
		color = sf::Color::Red;


	cell[0].color = color;
	cell[1].color = color;
	cell[2].color = color;
	cell[3].color = color;

	window.draw(cell);
}

void theCells::setMid(int i, int j)
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

	/*
	if ((nbr1 == 1) && (nbr2 == 8))
		this->setCell(i, j, 2);
	else if ((nbr1 == 8) && (nbr2 == 1))
		this->setCell(i, j, 1);
	*/
	if (chance < (double)nbr1/9.0)
		this->setCell(i, j, 1);
	else if (chance > (1.0-(double)nbr2/9.7))
		this->setCell(i, j, 2);

}

void theCells::setLeft(int i, int j)
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

void theCells::setRight(int i, int j)
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

void theCells::setTop(int i, int j)
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

void theCells::setBot(int i, int j)
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

void theCells::setTopLeft(int i, int j)
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

void theCells::setTopRight(int i, int j)
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

void theCells::setBotRight(int i, int j)
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

void theCells::setBotLeft(int i, int j)
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
