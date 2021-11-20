#pragma once
#include <SFML/Graphics.hpp>
class Animation
{
	bool flipped;

	//When this iterator reaches the animation speed, we change the frame and reset the iterator.
	unsigned short animation_iterator;
	//The higher the value, the slower the animation.
	unsigned short animation_speed;
	unsigned short current_frame;
	//To make things easier, each image file will contain 1 animation. So that the frame heights are the same.
	unsigned short frame_width;
	//We can find this by dividing the width of the image by the frame width.
	unsigned short total_frames;

	sf::Sprite sprite;

	sf::Texture texture;
public:
	Animation(unsigned short i_animation_speed, unsigned short i_frame_width, const std::string& i_texture_location);

	void draw(short i_x, short i_y, sf::RenderWindow& i_window);
	void set_animation_speed(unsigned short i_animation_speed);
	void set_flipped(bool i_value);
	void update();
};