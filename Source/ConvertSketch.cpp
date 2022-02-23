#include <array>
#include <cmath>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "Headers/Animation.hpp"
#include "Headers/Global.hpp"
#include "Headers/MapManager.hpp"
#include "Headers/Mushroom.hpp"
#include "Headers/Mario.hpp"
#include "Headers/Enemy.hpp"
#include "Headers/Goomba.hpp"
#include "Headers/Koopa.hpp"
#include "Headers/ConvertSketch.hpp"

//One person asked, "Why don't you use Tiled Map Editor?"
//My answer is, "Why should I work hard, when I don't have to work hard?"
void convert_sketch(const unsigned char i_current_level, unsigned short& i_level_finish, std::vector<std::shared_ptr<Enemy>>& i_enemies, sf::Color& i_background_color, MapManager& i_map_manager, Mario& i_mario)
{
	unsigned short map_height;

	i_map_manager.update_map_sketch(i_current_level);
	i_map_manager.set_map_size(i_map_manager.get_map_sketch_width());

	//We divide the height by 3 because the sketch stores the level as 3 layers: blocks, entities, and background tiles.
	map_height = floor(i_map_manager.get_map_sketch_height() / 3.f);

	i_background_color = i_map_manager.get_map_sketch_pixel(0, i_map_manager.get_map_sketch_height() - 1);

	for (unsigned short a = 0; a < i_map_manager.get_map_sketch_width(); a++)
	{
		for (unsigned short b = 0; b < 2 * map_height; b++)
		{
			sf::Color pixel = i_map_manager.get_map_sketch_pixel(a, b);

			if (b < map_height)
			{
				if (sf::Color(182, 73, 0) == pixel)
				{
					i_map_manager.set_map_cell(a, b, Cell::Brick);
				}
				else if (sf::Color(255, 255, 0) == pixel)
				{
					i_map_manager.set_map_cell(a, b, Cell::Coin);
				}
				else if (sf::Color(0, 146, 0) == pixel || sf::Color(0, 182, 0) == pixel || sf::Color(0, 219, 0) == pixel)
				{
					//Multiple colors, because we need to know which part of the pipe we need to draw.
					i_map_manager.set_map_cell(a, b, Cell::Pipe);
				}
				else if (sf::Color(255, 73, 85) == pixel || sf::Color(255, 146, 85) == pixel)
				{
					i_map_manager.set_map_cell(a, b, Cell::QuestionBlock);
				}
				else if (sf::Color(0, 0, 0) == pixel || sf::Color(146, 73, 0) == pixel)
				{
					i_map_manager.set_map_cell(a, b, Cell::Wall);
				}
				else
				{
					i_map_manager.set_map_cell(a, b, Cell::Empty);

					if (sf::Color(0, 255, 255) == pixel)
					{
						i_level_finish = a;
					}
				}
			}
			else
			{
				if (sf::Color(255, 0, 0) == pixel)
				{
					i_mario.set_position(CELL_SIZE * a, CELL_SIZE * (b - map_height));
				}
				else if (sf::Color(182, 73, 0) == pixel)
				{
					i_enemies.push_back(std::make_shared<Goomba>(sf::Color(0, 0, 85) == i_background_color, CELL_SIZE * a, CELL_SIZE * (b - map_height)));
				}
				else if (sf::Color(0, 219, 0) == pixel)
				{
					i_enemies.push_back(std::make_shared<Koopa>(sf::Color(0, 0, 85) == i_background_color, CELL_SIZE * a, CELL_SIZE * (b - map_height)));
				}
			}
		}
	}
}
