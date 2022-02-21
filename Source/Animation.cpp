#include <SFML/Graphics.hpp>

#include "Headers/Animation.hpp"

Animation::Animation(const unsigned short i_frame_width, const std::string& i_texture_location, const unsigned short i_animation_speed) :
	flipped(0),
	animation_iterator(0),
	animation_speed(std::max<unsigned short>(1, i_animation_speed)),
	current_frame(0),
	frame_width(i_frame_width)
{
	texture.loadFromFile(i_texture_location);

	total_frames = texture.getSize().x / frame_width;
}

void Animation::draw(sf::RenderWindow& i_window)
{
	sprite.setTexture(texture);

	if (0 == flipped)
	{
		sprite.setTextureRect(sf::IntRect(current_frame * frame_width, 0, frame_width, texture.getSize().y));
	}
	else
	{
		//This is why I love SFML.
		//It allows you to read the texture from right to left using negative numbers.
		sprite.setTextureRect(sf::IntRect(frame_width * (1 + current_frame), 0, -frame_width, texture.getSize().y));
	}

	i_window.draw(sprite);
}

void Animation::set_animation_speed(const unsigned short i_animation_speed)
{
	animation_speed = std::max<unsigned short>(1, i_animation_speed);
}

void Animation::set_flipped(const bool i_value)
{
	flipped = i_value;
}

void Animation::set_position(const short i_x, const short i_y)
{
	sprite.setPosition(i_x, i_y);
}

void Animation::set_texture_location(const std::string& i_texture_location)
{
	texture.loadFromFile(i_texture_location);
}

void Animation::update()
{
	while (animation_iterator >= animation_speed)
	{
		animation_iterator -= animation_speed;

		current_frame = (1 + current_frame) % total_frames;
	}

	animation_iterator++;
}