#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
class Animation
{
	bool flipped;

	//We'll increment the current_frame as soon as this iterator reaches the animation_speed.
	unsigned short animation_iterator;
	//After how many frames do we change the current_frame?
	unsigned short animation_speed;
	unsigned short current_frame;
	unsigned short frame_width;
	unsigned short total_frames;

	sf::Sprite sprite;

	sf::Texture texture;
public:
	Animation(const unsigned short i_frame_width, const std::string& i_texture_location, const unsigned short i_animation_speed = 1);

	void draw(sf::RenderWindow& i_window);
	void set_animation_speed(const unsigned short i_animation_speed);
	void set_flipped(const bool i_value);
	void set_position(const short i_x, const short i_y);
	void set_texture_location(const std::string& i_texture_location);
	void update();
};
