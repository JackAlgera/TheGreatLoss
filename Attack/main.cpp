#include <SFML\Graphics.hpp>

#include <Windows.h>
#include <ctime>
#include "theCells.h"
#include <iostream>
#include <cstdlib> 

/*
Program that simulates a battle between two sides, one sides evidently having the upper hand
The simulation uses a matrix to determine the color of each square.
The probability of a square turning into a certain color depends on it's color and the amount of neighbors that square has for a given color.

Author : Jacobus ALGERA
*/

const int nbrCells = 100;
const int cellSize = 8;
const sf::Vector2i screenDimensions(nbrCells*cellSize + nbrCells + 1, nbrCells*cellSize + nbrCells + 1);


int main()
{
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(screenDimensions.x, screenDimensions.y), "The great loss - Paused", sf::Style::Close | sf::Style::Titlebar);	//Creates the window 

	theCells cells(nbrCells, cellSize);
	cells.setCell(0, 0, 2);								//Initial position of team 2
	cells.setCell(99, 99, 1);							//Initial position of team 1
	cells.updateMatriceCells();

	printf("Witness the fall of an army\n\nPress P to pause, O to restart the simulation and R to start with random starting points\n\nAuthor : Jacobus Algera");

	bool gamePause = true;						

	while (window.isOpen())								//Main simulation loop
	{
		sf::Event evnt;
		while (window.pollEvent(evnt))					//Handles the events
		{
			if (evnt.type == sf::Event::Closed)
			{
				window.close();
			}
			if (evnt.type == sf::Event::TextEntered)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))	//Keyboard events
				{
					gamePause = !gamePause;
					if (gamePause)
					{
						window.setTitle("The great loss - Paused");
					}
					else
					{
						window.setTitle("The great loss");
					}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
				{
					cells.restart(false, window);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
				{
					cells.restart(true, window);
				}
			}
		}

		if (!gamePause)
		{
			cells.update();
		}
		cells.draw(window);
		window.display();
		Sleep(5);
		window.clear();
	}
}