//I didn't use the constants from "Global.hpp" to make this class more universal.
#include <SFML/Graphics.hpp>

#include "Headers/Animation.hpp"

Animation::Animation(unsigned short i_animation_speed, unsigned short i_frame_width, const std::string& i_texture_location) :
	flipped(0),
	animation_iterator(0),
	animation_speed(std::max<unsigned short>(1, i_animation_speed)),
	current_frame(0),
	frame_width(i_frame_width)
{
	texture.loadFromFile(i_texture_location);

	total_frames = texture.getSize().x / frame_width;
}

void Animation::draw(short i_x, short i_y, sf::RenderWindow& i_window)
{
	sprite.setPosition(i_x, i_y);
	sprite.setTexture(texture);

	if (0 == flipped)
	{
		sprite.setTextureRect(sf::IntRect(current_frame * frame_width, 0, frame_width, texture.getSize().y));
	}
	else
	{
		//To flip the sprite, we're gonna read the texture from right to left.
		sprite.setTextureRect(sf::IntRect(frame_width * (1 + current_frame), 0, -frame_width, texture.getSize().y));
	}

	i_window.draw(sprite);
}

void Animation::set_animation_speed(unsigned short i_animation_speed)
{
	animation_speed = std::max<unsigned short>(1, i_animation_speed);
}

void Animation::set_flipped(bool i_value)
{
	flipped = i_value;
}

void Animation::update()
{
	animation_iterator++;

	while (animation_iterator >= animation_speed)
	{
		animation_iterator -= animation_speed;

		current_frame = (1 + current_frame) % total_frames;
	}
}