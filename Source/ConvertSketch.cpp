#include <array>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "Headers/Animation.hpp"
#include "Headers/Global.hpp"
#include "Headers/Mario.hpp"
#include "Headers/Goomba.hpp"
#include "Headers/ConvertSketch.hpp"

Map convert_sketch(std::vector<Goomba>& i_goombas, const sf::Image& i_map_sketch, Mario& i_mario)
{
	//The map sketch is divided into 3 sections:
	//1) Placement of all the important blocks (walls, bricks, question blocks, etc.).
	//2) Placement of entities.
	//3) Placement of background tiles.
	unsigned short map_height = floor(i_map_sketch.getSize().y / 3.f);

	Map output_map(i_map_sketch.getSize().x);

	for (unsigned short a = 0; a < i_map_sketch.getSize().x; a++)
	{
		for (unsigned short b = 0; b < 2 * map_height; b++)
		{
			sf::Color pixel = i_map_sketch.getPixel(a, b);

			//First we're gonna place the blocks.
			if (b < map_height)
			{
				if (sf::Color(182, 73, 0) == pixel)
				{
					output_map[a][b] = Cell::Brick;
				}
				else if (sf::Color(0, 182, 0) == pixel)
				{
					output_map[a][b] = Cell::Pipe;
				}
				else if (sf::Color(255, 146, 85) == pixel)
				{
					output_map[a][b] = Cell::QuestionBlock;
				}
				else if (sf::Color(0, 0, 0) == pixel || sf::Color(146, 73, 0) == pixel)
				{
					output_map[a][b] = Cell::Wall;
				}
				else
				{
					output_map[a][b] = Cell::Empty;
				}
			}
			else //Then we're gonna place the entities.
			{
				if (sf::Color(255, 0, 0) == pixel)
				{
					i_mario.set_position(CELL_SIZE * a, CELL_SIZE * (b - map_height));
				}
				else if (sf::Color(182, 73, 0) == pixel)
				{
					i_goombas.push_back(Goomba(CELL_SIZE * a, CELL_SIZE * (b - map_height)));
				}
			}
		}
	}

	return output_map;
}
