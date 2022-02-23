#include <array>
#include <chrono>
#include <cmath>
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

int main()
{
	unsigned char current_level = 0;

	unsigned short level_finish = 0;

	//We'll use this to make the game framerate-independent.
	std::chrono::microseconds lag(0);

	std::chrono::steady_clock::time_point previous_time;

	//Using smart pointer because I'm smart.
	//(Because we need to store both Goomba and Koopa objects in the same vector).
	std::vector<std::shared_ptr<Enemy>> enemies;

	sf::Color background_color = sf::Color(0, 219, 255);

	sf::Event event;

	sf::RenderWindow window(sf::VideoMode(SCREEN_RESIZE * SCREEN_WIDTH, SCREEN_RESIZE * SCREEN_HEIGHT), "Super Mario Bros", sf::Style::Close);
	window.setPosition(sf::Vector2i(window.getPosition().x, window.getPosition().y - 90));

	sf::View view(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

	MapManager map_manager;

	Mario mario;

	convert_sketch(current_level, level_finish, enemies, background_color, map_manager, mario);

	previous_time = std::chrono::steady_clock::now();

	while (1 == window.isOpen())
	{
		std::chrono::microseconds delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time);

		lag += delta_time;

		previous_time += delta_time;

		//Soooooo, how was your day?
		//Mine was good. I ate some cookies.
		//Then I watched Youtube.
		//...
		//...
		//...
		//Yeah.

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
								enemies.clear();

								mario.reset();

								convert_sketch(current_level, level_finish, enemies, background_color, map_manager, mario);
							}
						}
					}
				}
			}

			//Once Mario goes beyond the finish, we move on to the next level.
			if (CELL_SIZE * level_finish <= mario.get_x())
			{
				current_level++;

				enemies.clear();

				mario.reset();

				convert_sketch(current_level, level_finish, enemies, background_color, map_manager, mario);
			}

			//Keeping Mario at the center of the view.
			view_x = std::clamp<int>(round(mario.get_x()) - 0.5f * (SCREEN_WIDTH - CELL_SIZE), 0, CELL_SIZE * map_manager.get_map_width() - SCREEN_WIDTH);

			map_manager.update();

			mario.update(view_x, map_manager);

			for (unsigned short a = 0; a < enemies.size(); a++)
			{
				enemies[a]->update(view_x, enemies, map_manager, mario);
			}

			for (unsigned short a = 0; a < enemies.size(); a++)
			{
				if (1 == enemies[a]->get_dead(1))
				{
					//We don't have to worry about memory leaks since we're using SMART POINTERS!
					enemies.erase(a + enemies.begin());

					a--;
				}
			}

			if (FRAME_DURATION > lag)
			{
				view.reset(sf::FloatRect(view_x, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

				window.setView(view);
				window.clear(background_color);

				//If the background color is sf::Color(0, 0, 85), the level is underground.
				map_manager.draw_map(1, sf::Color(0, 0, 85) == background_color, view_x, window);

				mario.draw_mushrooms(view_x, window);

				map_manager.draw_map(0, sf::Color(0, 0, 85) == background_color, view_x, window);

				for (unsigned short a = 0; a < enemies.size(); a++)
				{
					enemies[a]->draw(view_x, window);
				}

				mario.draw(window);

				window.display();
			}
		}
	}
}
