#include <array>
#include <chrono>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Headers/Global.hpp"
#include "Headers/DrawMap.hpp"

void draw_map(unsigned i_view_x, const sf::Image& i_map_sketch, sf::RenderWindow& i_window, const sf::Texture& i_map_texture, const Map& i_map)
{
	//OPTIMIZATION!
	//We'll only draw the part of the map that is visible on the screen.
	unsigned short map_end = ceil((i_view_x + SCREEN_WIDTH) / static_cast<float>(CELL_SIZE));
	unsigned short map_height = floor(i_map_sketch.getSize().y / 3.f);
	unsigned short map_start = floor(i_view_x / static_cast<float>(CELL_SIZE));

	sf::Sprite cell_sprite(i_map_texture);

	for (unsigned short a = map_start; a < map_end; a++)
	{
		for (unsigned short b = 0; b < map_height; b++)
		{
			unsigned short sprite_x = 0;
			unsigned short sprite_y = 0;

			sf::Color pixel = i_map_sketch.getPixel(a, b + 2 * map_height);
			sf::Color pixel_down = sf::Color(0, 0, 0, 0);
			sf::Color pixel_left = sf::Color(0, 0, 0, 0);
			sf::Color pixel_right = sf::Color(0, 0, 0, 0);
			sf::Color pixel_up = sf::Color(0, 0, 0, 0);

			cell_sprite.setPosition(CELL_SIZE * a, CELL_SIZE * b);
			
			//Ignore the empty pixels.
			if (255 == pixel.a)
			{
				//Here we're getting pixels around the pixel we're currently checking.
				if (0 < a)
				{
					pixel_left = i_map_sketch.getPixel(a - 1, b + 2 * map_height);
				}

				if (0 < b)
				{
					pixel_up = i_map_sketch.getPixel(a, b + 2 * map_height - 1);
				}

				if (a < i_map_sketch.getSize().x - 1)
				{
					pixel_right = i_map_sketch.getPixel(1 + a, b + 2 * map_height);
				}

				if (b < map_height - 1)
				{
					pixel_down = i_map_sketch.getPixel(a, 1 + b + 2 * map_height);
				}

				//Nothing complicated here.
				//Just a bunch of if's and else's that determine the position of the tile we're gonna draw.
				//Yeah, I know it's bad.
				if (sf::Color(255, 255, 255) == pixel) //Clouds
				{
					sprite_x = 8;

					if (sf::Color(255, 255, 255) == pixel_up)
					{
						sprite_y = 1;
					}

					if (sf::Color(255, 255, 255) == pixel_left)
					{
						if (sf::Color(255, 255, 255) != pixel_right)
						{
							sprite_x = 9;
						}
					}
					else if (sf::Color(255, 255, 255) == pixel_right)
					{
						sprite_x = 7;
					}
				}
				else if (sf::Color(146, 219, 0) == pixel) //Grass
				{
					sprite_x = 5;

					if (sf::Color(146, 219, 0) == pixel_left)
					{
						if (sf::Color(146, 219, 0) != pixel_right)
						{
							sprite_x = 6;
						}
					}
					else if (sf::Color(146, 219, 0) == pixel_right)
					{
						sprite_x = 4;
					}
				}
				else if (sf::Color(0, 73, 0) == pixel) //Hills outline
				{
					sprite_y = 1;

					if (sf::Color(0, 109, 0) == pixel_left)
					{
						if (sf::Color(0, 109, 0) != pixel_right)
						{
							sprite_x = 2;
						}
					}
					else if (sf::Color(0, 109, 0) == pixel_right)
					{
						sprite_x = 1;
					}
				}
				else if (sf::Color(0, 109, 0) == pixel) //Hills
				{
					sprite_x = 4;
					sprite_y = 1;

					if (sf::Color(0, 73, 0) == pixel_left)
					{
						sprite_x = 3;
					}
					else if (sf::Color(0, 73, 0) == pixel_right)
					{
						sprite_x = 5;
					}
				}
				else if (sf::Color(109, 255, 85) == pixel) //Flagpole
				{
					sprite_x = 12;

					if (sf::Color(109, 255, 85) == pixel_up)
					{
						sprite_y = 1;
					}
				}

				cell_sprite.setTextureRect(sf::IntRect(CELL_SIZE * sprite_x, CELL_SIZE * sprite_y, CELL_SIZE, CELL_SIZE));

				i_window.draw(cell_sprite);
			}

			//Yes, we're drawing the blocks separately from the background tiles.
			if (Cell::Empty != i_map[a][b])
			{
				if (Cell::Pipe == i_map[a][b]) //Pipes
				{
					if (Cell::Pipe == i_map[a][b - 1])
					{
						sprite_y = 1;
					}
					else
					{
						sprite_y = 0;
					}

					if (Cell::Pipe == i_map[a - 1][b])
					{
						sprite_x = 11;
					}
					else
					{
						sprite_x = 10;
					}
				}
				else if (Cell::QuestionBlock == i_map[a][b]) //Question blocks
				{
					sprite_x = 1;
					sprite_y = 0;
				}
				else if (Cell::Wall == i_map[a][b])
				{
					sprite_y = 0;

					if (sf::Color(0, 0, 0) == i_map_sketch.getPixel(a, b)) //Walls
					{
						sprite_x = 2;
					}
					else //Solid blocks
					{
						sprite_x = 3;
					}
				}

				cell_sprite.setTextureRect(sf::IntRect(CELL_SIZE * sprite_x, CELL_SIZE * sprite_y, CELL_SIZE, CELL_SIZE));

				i_window.draw(cell_sprite);
			}
		}
	}
}
