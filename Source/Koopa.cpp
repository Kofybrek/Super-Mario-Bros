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
#include "Headers/Koopa.hpp"

Koopa::Koopa(const bool i_underground, const float i_x, const float i_y) :
	Enemy(i_x, i_y),
	check_collision(1),
	flipped(1),
	no_collision_dying(0),
	underground(i_underground),
	state(0),
	get_out_timer(KOOPA_GET_OUT_DURATION),
	get_out_animation(CELL_SIZE, "Resources/Images/KoopaGetOut.png", KOOPA_GET_OUT_ANIMATION_SPEED),
	walk_animation(CELL_SIZE, "Resources/Images/KoopaWalk.png", KOOPA_WALK_ANIMATION_SPEED)
{
	horizontal_speed = -KOOPA_SPEED;

	if (0 == underground)
	{
		texture.loadFromFile("Resources/Images/KoopaShell.png");
	}
	else
	{
		texture.loadFromFile("Resources/Images/UndergroundKoopaShell.png");

		get_out_animation.set_texture_location("Resources/Images/UndergroundKoopaGetOut.png");
		walk_animation.set_texture_location("Resources/Images/UndergroundKoopaWalk.png");
	}
}

bool Koopa::get_dead(const bool i_deletion) const
{
	if (1 == i_deletion)
	{
		return dead;
	}
	else
	{
		return dead || no_collision_dying;
	}
}

void Koopa::die(const unsigned char i_death_type)
{
	switch (i_death_type)
	{
		case 0:
		{
			//Instant death.
			dead = 1;

			break;
		}
		case 2:
		{
			//Dying from Koopa's shell.
			no_collision_dying = 1;

			vertical_speed = 0.5f * MARIO_JUMP_SPEED;

			if (0 == underground)
			{
				texture.loadFromFile("Resources/Images/KoopaDeath.png");
			}
			else
			{
				texture.loadFromFile("Resources/Images/UndergroundKoopaDeath.png");
			}
		}
	}
}

void Koopa::draw(const unsigned i_view_x, sf::RenderWindow& i_window)
{
	if (-CELL_SIZE < round(y) && round(x) > static_cast<int>(i_view_x) - CELL_SIZE && round(x) < SCREEN_WIDTH + i_view_x && round(y) < SCREEN_HEIGHT)
	{
		if (0 < state || 1 == no_collision_dying)
		{
			if (0 == get_dead(0) && 1 == state && get_out_timer <= 0.25f * KOOPA_GET_OUT_DURATION)
			{
				get_out_animation.set_position(round(x), round(y));
				get_out_animation.draw(i_window);
			}
			else
			{
				sprite.setPosition(round(x), round(y));
				sprite.setTexture(texture);

				i_window.draw(sprite);
			}
		}
		else
		{
			walk_animation.set_flipped(flipped);
			walk_animation.set_position(round(x), round(y));
			walk_animation.draw(i_window);
		}
	}
}

void Koopa::update(const unsigned i_view_x, const std::vector<std::shared_ptr<Enemy>>& i_enemies, const MapManager& i_map_manager, Mario& i_mario)
{
	//I've already explained most of the code here in the Mario and Goomba classes.
	//I'm so bad at writing comments lol.

	if (-CELL_SIZE < y && x >= static_cast<int>(i_view_x) - CELL_SIZE - ENTITY_UPDATE_AREA && x < ENTITY_UPDATE_AREA + SCREEN_WIDTH + i_view_x && y < SCREEN_HEIGHT)
	{
		vertical_speed = std::min(GRAVITY + vertical_speed, MAX_VERTICAL_SPEED);

		if (0 == get_dead(0))
		{
			std::vector<unsigned char> collision;

			sf::FloatRect hit_box = get_hit_box();

			hit_box.top += vertical_speed;

			collision = i_map_manager.map_collision({Cell::ActivatedQuestionBlock, Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, hit_box);

			if (0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
			{
				return 0 == i_value;
			}))
			{
				if (0 > vertical_speed)
				{
					y = CELL_SIZE * (1 + floor((vertical_speed + y) / CELL_SIZE));
				}
				else
				{
					y = CELL_SIZE * (ceil((vertical_speed + y) / CELL_SIZE) - 1);
				}

				vertical_speed = 0;
			}
			else
			{
				bool changed = 0;

				for (unsigned short a = 0; a < i_enemies.size(); a++)
				{
					if (shared_from_this() != i_enemies[a] && 0 == i_enemies[a]->get_dead(0) && 1 == hit_box.intersects(i_enemies[a]->get_hit_box()))
					{
						changed = 1;

						if (0 > vertical_speed)
						{
							y = i_enemies[a]->get_hit_box().top + i_enemies[a]->get_hit_box().height;
						}
						else
						{
							y = i_enemies[a]->get_hit_box().top - CELL_SIZE;
						}

						vertical_speed = 0;

						break;
					}
				}

				if (0 == changed)
				{
					y += vertical_speed;
				}
			}

			hit_box = get_hit_box();
			hit_box.left += horizontal_speed;

			collision = i_map_manager.map_collision({Cell::ActivatedQuestionBlock, Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, hit_box);

			if (0 == no_collision_dying && 0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
			{
				return 0 == i_value;
			}))
			{
				if (0 < horizontal_speed)
				{
					x = CELL_SIZE * (ceil((horizontal_speed + x) / CELL_SIZE) - 1);
				}
				else
				{
					x = CELL_SIZE * (1 + floor((horizontal_speed + x) / CELL_SIZE));
				}

				horizontal_speed *= -1;
			}
			else
			{
				bool changed = 0;

				for (unsigned short a = 0; a < i_enemies.size(); a++)
				{
					if (shared_from_this() != i_enemies[a] && 0 == i_enemies[a]->get_dead(0) && 1 == hit_box.intersects(i_enemies[a]->get_hit_box()))
					{
						if (0 == state)
						{
							changed = 1;

							horizontal_speed *= -1;
						}
						else if (2 == state)
						{
							i_enemies[a]->die(2);
						}

						break;
					}
				}

				if (0 == changed)
				{
					x += horizontal_speed;
				}
			}

			if (0 == i_mario.get_dead() && 1 == get_hit_box().intersects(i_mario.get_hit_box()))
			{
				if (1 == check_collision)
				{
					if (0 == state)
					{
						//Turning into a shell.
						if (0 < i_mario.get_vertical_speed())
						{
							//We check the collision only once after we collide with Mario.
							check_collision = 0;

							horizontal_speed = 0;

							state = 1;

							get_out_timer = KOOPA_GET_OUT_DURATION;

							i_mario.set_vertical_speed(0.5f * MARIO_JUMP_SPEED);
						}
						else
						{
							i_mario.die(0);
						}
					}
					//Start sliding.
					else if (1 == state)
					{
						check_collision = 0;

						state = 2;

						//The direction is based on where Mario kicked Koopa.
						if (x < i_mario.get_x())
						{
							horizontal_speed = -KOOPA_SHELL_SPEED;
						}
						else
						{
							horizontal_speed = KOOPA_SHELL_SPEED;
						}

						if (0 < i_mario.get_vertical_speed())
						{
							i_mario.set_vertical_speed(0.5f * MARIO_JUMP_SPEED);
						}
					}
					else
					{
						//Stop sliding.
						if (0 < i_mario.get_vertical_speed())
						{
							check_collision = 0;

							horizontal_speed = 0;

							state = 1;

							get_out_timer = KOOPA_GET_OUT_DURATION;

							i_mario.set_vertical_speed(0.5f * MARIO_JUMP_SPEED);
						}
						//Don't stand in front of a sliding shell.
						else if ((0 < horizontal_speed && x < i_mario.get_x()) ||
								 (0 > horizontal_speed && x > i_mario.get_x()))
						{
							i_mario.die(0);
						}
					}
				}
			}
			else
			{
				//Once Mario stops intersecting with Koopa, start checking for collisions again.
				check_collision = 1;
			}

			if (0 < horizontal_speed)
			{
				flipped = 0;
			}
			else if (0 > horizontal_speed)
			{
				flipped = 1;
			}

			//Koopa can get out of his shell here.
			if (1 == state)
			{
				get_out_timer--;
				
				if (0 == get_out_timer)
				{
					state = 0;

					if (0 == flipped)
					{
						horizontal_speed = KOOPA_SPEED;
					}
					else
					{
						horizontal_speed = -KOOPA_SPEED;
					}
				}
				else
				{
					get_out_animation.update();
				}
			}

			walk_animation.update();
		}
		else
		{
			y += vertical_speed;
		}
	}

	if (SCREEN_HEIGHT <= y)
	{
		die(0);
	}
}
