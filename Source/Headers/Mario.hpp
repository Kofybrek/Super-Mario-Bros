#pragma once

class Mario
{
	bool dead;
	bool flipped;
	bool on_ground;

	float horizontal_speed;
    float last_horizontal_speed;
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
    sf::Music music;
    sf::Music other_music;
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
    unsigned char get_jump_timer();
    void set_jump_timer(unsigned char i_value);
    void set_horizontal_speed(float i_value);
    void update(const Map& i_map, const sf::Event & i_event, bool moving = 0);
    void go_right(const Map& i_map);
    void start_music();
    void start_other_music(std::string path);
	sf::FloatRect get_hit_box() const;
};
