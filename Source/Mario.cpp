#include <array>
#include <chrono>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Headers/Animation.hpp"
#include "Headers/Global.hpp"
#include "Headers/MapManager.hpp"
#include "Headers/Mushroom.hpp"
#include "Headers/Mario.hpp"

Mario::Mario() :
	crouching(0),
	dead(0),
	flipped(0),
	on_ground(0),
	enemy_bounce_speed(0),
	horizontal_speed(0),
	vertical_speed(0),
	x(0),
	y(0),
	jump_timer(0),
	powerup_state(0),
	death_timer(MARIO_DEATH_DURATION),
	growth_timer(0),
	invincible_timer(0),
	big_walk_animation(CELL_SIZE, "Resources/Images/BigMarioWalk.png", MARIO_WALK_ANIMATION_SPEED),
	walk_animation(CELL_SIZE, "Resources/Images/MarioWalk.png", MARIO_WALK_ANIMATION_SPEED)
{
	texture.loadFromFile("Resources/Images/MarioIdle.png");

	sprite.setTexture(texture);
}

bool Mario::get_dead() const
{
	return dead;
}

float Mario::get_vertical_speed() const
{
	return vertical_speed;
}

float Mario::get_x() const
{
	return x;
}

void Mario::die(const bool i_instant_death)
{
	//Mario instantly dies and it doesn't matter if he's big or small.
	if (1 == i_instant_death)
	{
		dead = 1;

		if (0 == powerup_state)
		{
			texture.loadFromFile("Resources/Images/MarioDeath.png");
		}
		else
		{
			texture.loadFromFile("Resources/Images/BigMarioDeath.png");
		}
	}
	//Mario dies, unless he's big.
	else if (0 == growth_timer && 0 == invincible_timer)
	{
		if (0 == powerup_state)
		{
			dead = 1;

			texture.loadFromFile("Resources/Images/MarioDeath.png");
		}
		else
		{
			powerup_state = 0;

			invincible_timer = MARIO_INVINCIBILITY_DURATION;

			if (0 == crouching)
			{
				y += CELL_SIZE;
			}
			else
			{
				crouching = 0;
			}
		}
	}
}

void Mario::draw(sf::RenderWindow& i_window)
{
	//When Mario is invincible, his sprite will blink.
	if (0 == invincible_timer / MARIO_BLINKING % 2)
	{
		bool draw_sprite = 1;
		//When Mario is growing, his sprite will switch between being big and small.
		bool draw_big = 0 == growth_timer / MARIO_BLINKING % 2;

		sprite.setPosition(round(x), round(y));

		if (0 == dead)
		{
			if (0 < powerup_state)
			{
				if (1 == crouching)
				{
					if (0 == draw_big)
					{
						texture.loadFromFile("Resources/Images/MarioIdle.png");
					}
					else
					{
						texture.loadFromFile("Resources/Images/BigMarioCrouch.png");
					}
				}
				else if (0 == on_ground)
				{
					if (0 == draw_big)
					{
						sprite.setPosition(round(x), CELL_SIZE + round(y));

						texture.loadFromFile("Resources/Images/MarioJump.png");
					}
					else
					{
						texture.loadFromFile("Resources/Images/BigMarioJump.png");
					}
				}
				else
				{
					if (0 == horizontal_speed)
					{
						if (0 == draw_big)
						{
							sprite.setPosition(round(x), CELL_SIZE + round(y));

							texture.loadFromFile("Resources/Images/MarioIdle.png");
						}
						else
						{
							texture.loadFromFile("Resources/Images/BigMarioIdle.png");
						}
					}
					else if ((0 < horizontal_speed && 0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
							  1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) ||
							 (0 > horizontal_speed && 0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
							  1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))

					{
						if (0 == draw_big)
						{
							sprite.setPosition(round(x), CELL_SIZE + round(y));

							texture.loadFromFile("Resources/Images/MarioBrake.png");
						}
						else
						{
							texture.loadFromFile("Resources/Images/BigMarioBrake.png");
						}
					}
					else
					{
						draw_sprite = 0;

						if (0 == draw_big)
						{
							walk_animation.set_flipped(flipped);
							walk_animation.set_position(round(x), CELL_SIZE + round(y));
							walk_animation.draw(i_window);
						}
						else
						{
							big_walk_animation.set_flipped(flipped);
							big_walk_animation.set_position(round(x), round(y));
							big_walk_animation.draw(i_window);
						}
					}
				}
			}
			else if (0 == on_ground)
			{
				texture.loadFromFile("Resources/Images/MarioJump.png");
			}
			else
			{
				if (0 == horizontal_speed)
				{
					texture.loadFromFile("Resources/Images/MarioIdle.png");
				}
				else if ((0 < horizontal_speed && 0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
						  1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) ||
						 (0 > horizontal_speed && 0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
						  1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))

				{
					texture.loadFromFile("Resources/Images/MarioBrake.png");
				}
				else
				{
					draw_sprite = 0;

					walk_animation.set_flipped(flipped);
					walk_animation.set_position(round(x), round(y));
					walk_animation.draw(i_window);
				}
			}
		}

		if (1 == draw_sprite)
		{
			if (0 == flipped)
			{
				sprite.setTextureRect(sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y));
			}
			else
			{
				sprite.setTextureRect(sf::IntRect(texture.getSize().x, 0, -static_cast<int>(texture.getSize().x), texture.getSize().y));
			}

			i_window.draw(sprite);
		}
	}
}

void Mario::draw_mushrooms(const unsigned i_view_x, sf::RenderWindow& i_window)
{
	for (Mushroom& mushroom : mushrooms)
	{
		mushroom.draw(i_view_x, i_window);
	}
}

void Mario::reset()
{
	crouching = 0;
	dead = 0;
	flipped = 0;
	on_ground = 0;

	enemy_bounce_speed = 0;
	horizontal_speed = 0;
	vertical_speed = 0;
	x = 0;
	y = 0;

	jump_timer = 0;
	powerup_state = 0;

	//If it wasn't for this, everything would be zerfect! (see what I did there?)
	//...
	//...
	//...
	//Yeah, I agree. That was bad.
	death_timer = MARIO_DEATH_DURATION;
	growth_timer = 0;
	invincible_timer = 0;

	mushrooms.clear();

	texture.loadFromFile("Resources/Images/MarioIdle.png");

	sprite.setTexture(texture);

	big_walk_animation.set_animation_speed(MARIO_WALK_ANIMATION_SPEED);
	big_walk_animation.set_flipped(0);

	walk_animation.set_animation_speed(MARIO_WALK_ANIMATION_SPEED);
	walk_animation.set_flipped(0);
}

void Mario::set_position(const float i_x, const float i_y)
{
	x = i_x;
	y = i_y;
}

void Mario::set_vertical_speed(const float i_value)
{
	enemy_bounce_speed = i_value;
}

void Mario::update(const unsigned i_view_x, MapManager& i_map_manager)
{
	//We make Mario bounce after updating all the enemies to prevent a bug (Go to Mario.hpp for explanation).
	if (0 != enemy_bounce_speed)
	{
		vertical_speed = enemy_bounce_speed;

		enemy_bounce_speed = 0;
	}

	for (Mushroom& mushroom : mushrooms)
	{
		mushroom.update(i_view_x, i_map_manager);
	}

	if (0 == dead)
	{
		bool moving = 0;

		//So basically, the map_collision function returns a vector of numbers. Each number is a binary representation of the collisions in a single row. And we're storing that vector in this vector.
		std::vector<unsigned char> collision;

		//Oh yeah, the map_collision function can also return the coordinates of cells intersecting the hitbox. We need that too.
		std::vector<sf::Vector2i> cells;

		sf::FloatRect hit_box = get_hit_box();

		on_ground = 0;

		if (0 == crouching)
		{
			if (0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
				1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				moving = 1;

				horizontal_speed = std::max(horizontal_speed - MARIO_ACCELERATION, -MARIO_WALK_SPEED);
			}

			if (0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
				1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				moving = 1;

				horizontal_speed = std::min(MARIO_ACCELERATION + horizontal_speed, MARIO_WALK_SPEED);
			}
		}

		if (0 == moving)
		{
			if (0 < horizontal_speed)
			{
				horizontal_speed = std::max<float>(0, horizontal_speed - MARIO_ACCELERATION);
			}
			else if (0 > horizontal_speed)
			{
				horizontal_speed = std::min<float>(0, MARIO_ACCELERATION + horizontal_speed);
			}
		}

		if (0 < powerup_state)
		{
			if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::C) || 1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				if (0 == crouching)
				{
					crouching = 1;

					y += CELL_SIZE;
				}
			}
			else if (1 == crouching)
			{
				hit_box.height += CELL_SIZE;
				hit_box.top -= CELL_SIZE;

				//Making sure we can stand up without hitting anything.
				collision = i_map_manager.map_collision({Cell::ActivatedQuestionBlock, Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, hit_box);

				if (1 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
				{
					return 0 == i_value;
				}))
				{
					crouching = 0;

					y -= CELL_SIZE;
				}
				else
				{
					collision = i_map_manager.map_collision({Cell::ActivatedQuestionBlock, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, hit_box);

					//But if it happens to be bricks, we'll destroy them.
					if (1 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
					{
						return 0 == i_value;
					}))
					{
						crouching = 0;

						y -= CELL_SIZE;

						i_map_manager.map_collision({Cell::Brick}, cells, hit_box);

						for (const sf::Vector2i& cell : cells)
						{
							i_map_manager.set_map_cell(cell.x, cell.y, Cell::Empty);
							i_map_manager.add_brick_particles(CELL_SIZE * cell.x, CELL_SIZE * cell.y);
						}
					}
				}
			}
		}

		hit_box = get_hit_box();
		hit_box.left += horizontal_speed;
		
		collision = i_map_manager.map_collision({Cell::ActivatedQuestionBlock, Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, hit_box);

		if (0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
		{
			return 0 == i_value;
		}))
		{
			moving = 0;

			if (0 < horizontal_speed)
			{
				x = CELL_SIZE * (ceil((horizontal_speed + x) / CELL_SIZE) - 1);
			}
			else if (0 > horizontal_speed)
			{
				x = CELL_SIZE * (1 + floor((horizontal_speed + x) / CELL_SIZE));
			}

			horizontal_speed = 0;
		}
		else
		{
			x += horizontal_speed;
		}

		hit_box = get_hit_box();
		hit_box.top++;

		collision = i_map_manager.map_collision({Cell::ActivatedQuestionBlock, Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, hit_box);
		
		if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || 1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			if (0 == vertical_speed && 0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
			{
				return 0 == i_value;
			}))
			{
				vertical_speed = MARIO_JUMP_SPEED;

				jump_timer = MARIO_JUMP_TIMER;
			}
			else if (0 < jump_timer) //The longer we press the jump button, the higher Mario jumps.
			{
				vertical_speed = MARIO_JUMP_SPEED;

				jump_timer--;
			}
			else
			{
				vertical_speed = std::min(GRAVITY + vertical_speed, MAX_VERTICAL_SPEED);
			}
		}
		else
		{
			vertical_speed = std::min(GRAVITY + vertical_speed, MAX_VERTICAL_SPEED);

			jump_timer = 0;
		}

		hit_box = get_hit_box();
		hit_box.top += vertical_speed;

		collision = i_map_manager.map_collision({Cell::ActivatedQuestionBlock, Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, hit_box);
		
		if (0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
		{
			return 0 == i_value;
		}))
		{
			if (0 > vertical_speed)
			{
				//Destroying bricks!!!!
				if (0 == crouching && 0 < powerup_state)
				{
					i_map_manager.map_collision({Cell::Brick}, cells, hit_box);

					for (const sf::Vector2i& cell : cells)
					{
						i_map_manager.set_map_cell(cell.x, cell.y, Cell::Empty);
						i_map_manager.add_brick_particles(CELL_SIZE * cell.x, CELL_SIZE * cell.y);
					}
				}

				i_map_manager.map_collision({Cell::QuestionBlock}, cells, hit_box);

				//Activating question blocks!!!!
				for (const sf::Vector2i& cell : cells)
				{
					i_map_manager.set_map_cell(cell.x, cell.y, Cell::ActivatedQuestionBlock);

					//It can be either a mushroom or a coin, depending on the color of the pixel in the sketch.
					if (sf::Color(255, 73, 85) == i_map_manager.get_map_sketch_pixel(cell.x, cell.y))
					{
						mushrooms.push_back(Mushroom(CELL_SIZE * cell.x, CELL_SIZE * cell.y));
					}
					else
					{
						i_map_manager.add_question_block_coin(CELL_SIZE * cell.x, CELL_SIZE * cell.y);
					}
				}

				y = CELL_SIZE * (1 + floor((vertical_speed + y) / CELL_SIZE));
			}
			else if (0 < vertical_speed)
			{
				y = CELL_SIZE * (ceil((vertical_speed + y) / CELL_SIZE) - 1);
			}

			jump_timer = 0;

			vertical_speed = 0;
		}
		else
		{
			y += vertical_speed;
		}

		if (0 == horizontal_speed)
		{
			if (1 == moving)
			{
				flipped = 1 - flipped;
			}
		}
		else if (0 < horizontal_speed)
		{
			flipped = 0;
		}
		else if (0 > horizontal_speed)
		{
			flipped = 1;
		}

		hit_box = get_hit_box();
		hit_box.top++;

		collision = i_map_manager.map_collision({Cell::ActivatedQuestionBlock, Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, hit_box);

		if (0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
		{
			return 0 == i_value;
		}))
		{
			on_ground = 1;
		}

		for (Mushroom& mushroom : mushrooms)
		{
			//Mushroom eating and becoming BIG, STRONG, MASCULINE!!!!
			if (1 == get_hit_box().intersects(mushroom.get_hit_box()))
			{
				mushroom.set_dead(1);

				if (0 == powerup_state)
				{
					powerup_state = 1;

					growth_timer = MARIO_GROWTH_DURATION;

					y -= CELL_SIZE;
				}
			}
		}

		if (0 < invincible_timer)
		{
			invincible_timer--;
		}

		hit_box = get_hit_box();

		i_map_manager.map_collision({Cell::Coin}, cells, hit_box);

		//Collecting coins.
		for (const sf::Vector2i& cell : cells)
		{
			i_map_manager.set_map_cell(cell.x, cell.y, Cell::Empty);
		}

		if (0 < growth_timer)
		{
			growth_timer--;
		}

		if (y >= SCREEN_HEIGHT - get_hit_box().height)
		{
			die(1);
		}

		if (0 == powerup_state)
		{
			walk_animation.set_animation_speed(MARIO_WALK_ANIMATION_SPEED * MARIO_WALK_SPEED / abs(horizontal_speed));
			walk_animation.update();
		}
		else
		{
			big_walk_animation.set_animation_speed(MARIO_WALK_ANIMATION_SPEED * MARIO_WALK_SPEED / abs(horizontal_speed));
			big_walk_animation.update();
		}
	}
	else
	{
		if (0 == death_timer)
		{
			vertical_speed = std::min(GRAVITY + vertical_speed, MAX_VERTICAL_SPEED);
			y += vertical_speed;
		}
		else if (1 == death_timer)
		{
			vertical_speed = MARIO_JUMP_SPEED;
		}

		death_timer = std::max(0, death_timer - 1);
	}

	//Deleting mushrooms from the vector.
	mushrooms.erase(remove_if(mushrooms.begin(), mushrooms.end(), [](const Mushroom& i_mushroom)
	{
		return 1 == i_mushroom.get_dead();
	}), mushrooms.end());
}

sf::FloatRect Mario::get_hit_box() const
{
	//The hitbox will be small if Mario is small or crouching.
	if (1 == crouching || 0 == powerup_state)
	{
		return sf::FloatRect(x, y, CELL_SIZE, CELL_SIZE);
	}
	else
	{
		return sf::FloatRect(x, y, CELL_SIZE, 2 * CELL_SIZE);
	}
}
