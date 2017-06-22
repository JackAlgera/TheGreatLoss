#include <SFML\Graphics.hpp>

#include <Windows.h>
#include <ctime>
#include "Grid.h"
#include <iostream>
#include <cstdlib> 

const int nbrCells = 100;
const int cellSize = 8;
const int slowDelay = 2; //in ms
const sf::Vector2i screenDimensions(nbrCells*cellSize + nbrCells + 1, nbrCells*cellSize + nbrCells + 1);


int main()
{
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(screenDimensions.x, screenDimensions.y), "The great loss - Paused", sf::Style::Close | sf::Style::Titlebar);

	Grid cells(nbrCells, cellSize);
	cells.setCell(2, 2, 2);
	cells.setCell(2, 3, 2);
	cells.setCell(3, 3, 2);
	cells.setCell(3, 2, 2);
	cells.setCell(nbrCells-1, nbrCells-1, 1);
	cells.updateMatriceCells();

	printf("Witness the fall of an army\n\nPress P to pause, O to restart the simulation and R to start with random starting points\n\nAuthor : Jacobus Algera");

	bool gamePause = true;

	while (window.isOpen())
	{
		sf::Event evnt;
		while (window.pollEvent(evnt))
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
		Sleep(slowDelay);
		window.clear();
	}
}