#include <array>
#include <chrono>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Headers/Animation.hpp"
#include "Headers/Global.hpp"
#include "Headers/MapCollision.hpp"
#include "Headers/Mario.hpp"

Mario::Mario() :
	//So many zeros!
	dead(0),
	flipped(0),
	on_ground(0),
	horizontal_speed(0),
	vertical_speed(0),
	x(0),
	y(0),
	jump_timer(0),
	death_timer(MARIO_DEATH_DURATION),
	walk_animation(MARIO_WALK_ANIMATION_SPEED, CELL_SIZE, "Resources/Images/MarioWalk.png")
{
	texture.loadFromFile("Resources/Images/MarioIdle.png");

	sprite.setTexture(texture);
}

bool Mario::get_dead() const
{
	return dead;
}

unsigned char Mario::get_jump_timer(){
    return this->jump_timer;
}
void Mario::set_jump_timer(unsigned char i_value){
    this->jump_timer = i_value;
}

float Mario::get_vertical_speed() const
{
	return vertical_speed;
}

float Mario::get_x() const
{
	return x;
}

void Mario::die()
{
	dead = 1;

	texture.loadFromFile("Resources/Images/MarioDeath.png");
    // Load a music to play
    if (music.openFromFile( "Resources/audio/die.wav")) {
        music.setLoop(false);
        music.play();
    }
}

void Mario::start_music(){
    if (music.openFromFile(+ "Resources/audio/bgm.wav")) {
        music.setLoop(true);
        music.play();
    }
}

void Mario::start_other_music(std::string path){
    if (other_music.openFromFile(path)) {
        other_music.setLoop(false);
        other_music.play();
    }
}

void Mario::draw(sf::RenderWindow& i_window)
{
	bool draw_sprite = 1;

	sprite.setPosition(round(x), round(y));

	if (0 == dead)
	{
		if (0 == on_ground)
		{
			texture.loadFromFile("Resources/Images/MarioJump.png");
		}
		else
		{
			if (0 == horizontal_speed)
			{
				texture.loadFromFile( "Resources/Images/MarioIdle.png");
			}
			//You have no idea how many hours I spent trying to figure this out.
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
				walk_animation.draw(round(x), round(y), i_window);
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

void Mario::reset()
{
	dead = 0;
	flipped = 0;
	on_ground = 0;

	horizontal_speed = 0;
	vertical_speed = 0;
	x = 0;
	y = 0;

	jump_timer = 0;
    this->start_music();
	death_timer = MARIO_DEATH_DURATION;

	texture.loadFromFile( "Resources/Images/MarioIdle.png");

	sprite.setTexture(texture);

	walk_animation.set_animation_speed(MARIO_WALK_ANIMATION_SPEED);
	walk_animation.set_flipped(0);
}

void Mario::set_position(float i_x, float i_y)
{
	x = i_x;
	y = i_y;
}

void Mario::set_vertical_speed(float i_value)
{
	vertical_speed = i_value;
}

void Mario::set_horizontal_speed(float i_value)
{
    horizontal_speed = i_value;
}

void Mario::update(const Map& i_map, const sf::Event & event, bool moving)
{
//    bool moving = 1;
    bool move_left = 0;
    bool move_right = 0;
    bool move_jump = 0;
	if (0 == dead)
	{

		unsigned char horizontal_collision;
		unsigned char vertical_collision;

		on_ground = 1;
        // move left
		if (0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
			1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            move_left = 1;
        }
        // macOS compatiable
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)
		{
            move_left = 1;
        }
        if (move_left) {
			moving = 1;
			horizontal_speed = std::max(horizontal_speed - MARIO_ACCELERATION, -MARIO_WALK_SPEED);
            last_horizontal_speed = horizontal_speed;
		}
        // move right
        
		if (0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
			1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            move_right = 1;
        }
        // macOS compatiable
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
        {
            move_right = 1;
        }
        if (move_right)
		{
			moving = 1;

			horizontal_speed = std::min(MARIO_ACCELERATION + horizontal_speed, MARIO_WALK_SPEED);
            last_horizontal_speed = horizontal_speed;
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
		
		//map_collision() returns a number between 0 and 15.
		//In binary it's from 0000 to 1111.
		//                                       0  0  0  0
		//             __________________________|  |  |  |______________________________
		//            |                        _____|  |_____________                   |
		//            |                       |                     |                   |
		//           \/                      \/                    \/                  \/
		//{bottom-right collision, bottom-left collision, top-right collision, top-left collision}

		horizontal_collision = map_collision(horizontal_speed + x, y, {Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, i_map);
//        std::cout << horizontal_collision << "\t speed: " << horizontal_speed << std::endl;

		if (0 < horizontal_collision)
		{
			moving = 0;

			//Here we're rounding Mario's position to the nearest cell.
			//5  - 0101 - left collision
			//10 - 1010 - right collision
			if (5 & ~horizontal_collision && 10 & horizontal_collision)
			{
				x = CELL_SIZE * (ceil((horizontal_speed + x) / CELL_SIZE) - 1);
			}
			else if (5 & horizontal_collision && 10 & ~horizontal_collision)
			{
				x = CELL_SIZE * (1 + floor((horizontal_speed + x) / CELL_SIZE));
			}

			horizontal_speed = 0;
		}
		else
		{
			x += horizontal_speed;
		}

		//Is Mario standing on the ground?
		vertical_collision = map_collision(x, 1 + y, {Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, i_map);
//        std::cout << "first: " << vertical_collision << std::endl;
		//I added the <Z> key because some people will say, "I LiKe tO PrEsS ThE <Z> KeY InStEaD Of tHe <Up> KeY!"
		if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
            1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            move_jump = 1;
        } else if  (event.type == sf::Event::KeyPressed &&
            (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Z))
        {
            move_jump = 1;
        }
        if (move_jump)
		{
//            horizontal_speed = 2;
//            if (horizontal_speed == 0) {
//                horizontal_speed = last_horizontal_speed * 2;
//                last_horizontal_speed /= 2;
//            }
			if (0 == vertical_speed && 0 < vertical_collision)
			{
				vertical_speed = MARIO_JUMP_SPEED;

				jump_timer = MARIO_JUMP_TIMER;
			}
			else if (0 < jump_timer) //This will make Mario jump higher the longer we press the jump key.
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

		vertical_collision = map_collision(x, vertical_speed + y, {Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, i_map);
//        std::cout << "horontal speed: " << horizontal_speed << std::endl;
//        std::cout << "vetical speed: " << vertical_speed << std::endl;
		if (0 < vertical_collision)
		{
			//3  - 0011 - top collision
			//12 - 1100 - bottom collision
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

		if (y >= SCREEN_HEIGHT - CELL_SIZE)
		{
			die();
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

		//Is Mario still standing on the ground after we've changed the vertical speed and the y-coordinate?
		vertical_collision = map_collision(x, 1 + y, {Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, i_map);

		if (0 < vertical_collision)
		{
			on_ground = 1;
		}

		walk_animation.set_animation_speed(MARIO_WALK_ANIMATION_SPEED * MARIO_WALK_SPEED / abs(horizontal_speed));
		walk_animation.update();
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
            this->reset();
		}
        

		death_timer = std::max(0, death_timer - 1);
	}
}

sf::FloatRect Mario::get_hit_box() const
{
	return sf::FloatRect(x, y, CELL_SIZE, CELL_SIZE);
}
