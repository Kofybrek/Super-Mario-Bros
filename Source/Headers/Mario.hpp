#pragma once

#include <SFML/Audio.hpp>

class Mario
{
	bool dead;
	bool flipped;
	bool on_ground;

	float horizontal_speed;
	float vertical_speed;
	float x;
	float y;

	//I explained this in "Global.hpp".
	unsigned char jump_timer;

	//This too.
	unsigned short death_timer;

	//We'll use sprite and texture for the still images.
	sf::Sprite sprite;

	sf::Texture texture;

  sf::SoundBuffer jump_buf;
  sf::Sound jump_snd;

  sf::SoundBuffer die_buf;
  sf::Sound die_snd;

	Animation walk_animation;
public:
	Mario();

	bool get_dead() const;

	float get_vertical_speed() const;
	float get_x() const;

	//Perhaps I should change the name of this function.
	void die();
	void draw(sf::RenderWindow& i_window);
	void reset();
	void set_position(float i_x, float i_y);
	void set_vertical_speed(float i_value);
	void update(const Map& i_map);

	sf::FloatRect get_hit_box() const;
};
