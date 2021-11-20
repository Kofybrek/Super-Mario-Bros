#include <array>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "Headers/Animation.hpp"
#include "Headers/Global.hpp"
#include "Headers/MapCollision.hpp"
#include "Headers/Mario.hpp"
#include "Headers/Goomba.hpp"

/*

This is supposed to be a pool with 5 swimmers.

     ____      ____      ____      ____     
____/    \____/    \____/    \____/    \____
                                            
_      ____      ____      __\o_     ____   
 \____/    \____/    \____/    \____/    \__
                                            
___      ____ _o/  ____      ____      ____ 
   \____/    \____/    \____/    \____/    \
                            \o.             
 ____      ____      ____      ____      ___
/    \\o/_/    \____/    \____/    \____/   

P.S. The 5th swimmer is drowning.

*/

Goomba::Goomba(float i_x, float i_y) :
	dead(0),
	horizontal_speed(-GOOMBA_SPEED),
	vertical_speed(0),
	x(i_x),
	y(i_y),
	death_timer(GOOMBA_DEATH_DURATION),
	walk_animation(GOOMBA_WALK_ANIMATION_SPEED, CELL_SIZE,   "Resources/Images/GoombaWalk.png")
{
	texture.loadFromFile("Resources/Images/GoombaDeath.png");
}

bool Goomba::get_dead() const
{
	return dead;
}

unsigned short Goomba::get_death_timer() const
{
	return death_timer;
}

void Goomba::die()
{
	dead = 1;
}

void Goomba::draw(unsigned i_view_x, sf::RenderWindow& i_window)
{
	//OPTIMIZATION!
	//We'll only draw goomba if it's visible on the screen.
	if (-CELL_SIZE < round(y) && round(x) > static_cast<int>(i_view_x) - CELL_SIZE && round(x) < SCREEN_WIDTH + i_view_x && round(y) < SCREEN_HEIGHT)
	{
		if (1 == dead)
		{
			sprite.setPosition(round(x), round(y));
			sprite.setTexture(texture);

			i_window.draw(sprite);
		}
		else
		{
			walk_animation.draw(round(x), round(y), i_window);
		}
	}
}

void Goomba::update(unsigned i_view_x, const std::vector<Goomba>& i_goombas, const Map& i_map, Mario& i_mario)
{
	//OPTIMIZATION!
	//We'll only update goomba if it's visible on the screen.
	if (-CELL_SIZE < y && x >= static_cast<int>(i_view_x) - CELL_SIZE - ENTITY_UPDATE_AREA && x < ENTITY_UPDATE_AREA + SCREEN_WIDTH + i_view_x && y < SCREEN_HEIGHT)
	{
		//I already explained this in "Mario.cpp".
		unsigned char vertical_collision;

		vertical_speed = std::min(GRAVITY + vertical_speed, MAX_VERTICAL_SPEED);

		sf::FloatRect vertical_hit_box(x, vertical_speed + y, CELL_SIZE, CELL_SIZE);

		vertical_collision = map_collision(x, vertical_speed + y, {Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, i_map);

		if (0 < vertical_collision)
		{
			if (3 & vertical_collision && 12 & ~vertical_collision)
			{
				y = CELL_SIZE * (1 + floor((vertical_speed + y) / CELL_SIZE));
			}
			else if (3 & ~vertical_collision && 12 & vertical_collision)
			{
				y = CELL_SIZE * (ceil((vertical_speed + y) / CELL_SIZE) - 1);
			}

			vertical_speed = 0;
		}
		else
		{
			y += vertical_speed;
		}

		if (0 == dead)
		{
			unsigned char horizontal_collision;

			sf::FloatRect horizontal_hit_box(horizontal_speed + x, y, CELL_SIZE, CELL_SIZE);

			horizontal_collision = map_collision(horizontal_speed + x, y, {Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, i_map);

			if (0 < horizontal_collision)
			{
				if (5 & ~horizontal_collision && 10 & horizontal_collision)
				{
					x = CELL_SIZE * (ceil((horizontal_speed + x) / CELL_SIZE) - 1);
				}
				else if (5 & horizontal_collision && 10 & ~horizontal_collision)
				{
					x = CELL_SIZE * (1 + floor((horizontal_speed + x) / CELL_SIZE));
				}

				horizontal_speed *= -1;
			}
			else
			{
				bool changed = 0;

				for (const Goomba& goomba : i_goombas)
				{
					//Making sure the goombas don't walk through each other.
					//I USED POINTERS!
					//        |
					//        |
					//       \/
					if (this != &goomba && 0 == goomba.get_dead() && 1 == horizontal_hit_box.intersects(goomba.get_hit_box()))
					{
						changed = 1;

						horizontal_speed *= -1;

						break;
					}
				}

				if (0 == changed)
				{
					x += horizontal_speed;
				}
			}

			if (0 == i_mario.get_dead() && (1 == horizontal_hit_box.intersects(i_mario.get_hit_box()) || 1 == vertical_hit_box.intersects(i_mario.get_hit_box())))
			{
				//If Mario's vertical speed is greater than 0, that means he's falling.
				//Therefore, if Mario touches the goomba while falling, the goomba will die.
				if (0 < i_mario.get_vertical_speed())
				{
                    i_mario.start_other_music( "Resources/audio/step.wav");
					i_mario.set_vertical_speed(0.5f * MARIO_JUMP_SPEED);
					die();
				}
				else
				{
					i_mario.die();
				}
			}

			walk_animation.update();
		}
		else
		{
			death_timer = std::max(0, death_timer - 1);
		}
	}

	//If goomba falls outside the map, it'll be removed.
	if (SCREEN_HEIGHT <= y)
	{
		dead = 1;

		death_timer = 0;
	}
}

sf::FloatRect Goomba::get_hit_box() const
{
	return sf::FloatRect(x, y, CELL_SIZE, CELL_SIZE);
}
