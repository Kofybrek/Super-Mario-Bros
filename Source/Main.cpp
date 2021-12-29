#include <array>
#include <cmath>
#include <chrono>
#include <algorithm>
#include <SFML/Graphics.hpp>

#include "Headers/Animation.hpp"
#include "Headers/Global.hpp"
#include "Headers/Mario.hpp"
#include "Headers/Goomba.hpp"
#include "Headers/ConvertSketch.hpp"
#include "Headers/DrawMap.hpp"

int main()
{
	//We'll use this to make the game framerate independent.
	std::chrono::microseconds lag(0);

	std::chrono::steady_clock::time_point previous_time;

	std::vector<Goomba> goombas;

	sf::Event event;

	sf::Image map_sketch;
	map_sketch.loadFromFile("Resources/Images/MapSketch.png");

	sf::RenderWindow window(sf::VideoMode(SCREEN_RESIZE * SCREEN_WIDTH, SCREEN_RESIZE * SCREEN_HEIGHT), "Super Mario Bros", sf::Style::Close);
	
	sf::Texture map_texture;
	map_texture.loadFromFile("Resources/Images/Map.png");

	sf::View view(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

	Map map;

	Mario mario;

	map = convert_sketch(goombas, map_sketch, mario);

	previous_time = std::chrono::steady_clock::now();

	while (1 == window.isOpen())
	{
		std::chrono::microseconds delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time);

		lag += delta_time;

		previous_time += delta_time;

		while (FRAME_DURATION <= lag)
		{
			unsigned view_x;

			lag -= FRAME_DURATION;

			while (1 == window.pollEvent(event))
			{
				switch (event.type)
				{
					case sf::Event::Closed:
					{
						window.close();

						break;
					}
					case sf::Event::KeyPressed:
					{
						switch (event.key.code)
						{
							case sf::Keyboard::Enter:
							{
								goombas.clear();

								mario.reset();

								map = convert_sketch(goombas, map_sketch, mario);
							}
						}
					}
				}
			}

			mario.update(map);

			view_x = std::clamp<int>(round(mario.get_x()) - 0.5f * (SCREEN_WIDTH - CELL_SIZE), 0, CELL_SIZE * map.size() - SCREEN_WIDTH);

			for (Goomba& goomba : goombas)
			{
				goomba.update(view_x, goombas, map, mario);
			}

			//We delete dead goombas for OPTIMIZATION!
			for (unsigned short a = 0; a < goombas.size(); a++)
			{
				//At first I tried to use iterators and pointers.
				//But then I kept getting errors.
				//So I decided to use numbers instead.
				if (0 == goombas[a].get_death_timer())
				{
					goombas.erase(a + goombas.begin());

					a--;
				}
			}

			if (FRAME_DURATION > lag)
			{
				view.reset(sf::FloatRect(view_x, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

				window.setView(view);
				window.clear(sf::Color(0, 219, 255));

				draw_map(view_x, map_sketch, window, map_texture, map);

				for (Goomba& goomba : goombas)
				{
					goomba.draw(view_x, window);
				}

				mario.draw(window);

				window.display();
			}
		}
	}
}
