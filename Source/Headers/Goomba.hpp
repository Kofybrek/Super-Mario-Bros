#pragma once

class Goomba
{
	bool dead;

	float horizontal_speed;
	float vertical_speed;
	float x;
	float y;

	//As soon as this timer reaches 0, we remove this goomba.
	unsigned short death_timer;

	sf::Sprite sprite;

	sf::Texture texture;

	Animation walk_animation;
public:
	Goomba(float i_x, float i_y);

	bool get_dead() const;

	unsigned short get_death_timer() const;

	//This sounds so dark.
	void die();
	void draw(unsigned i_view_x, sf::RenderWindow& i_window);
	void update(unsigned i_view_x, const std::vector<Goomba>& i_goombas, const Map& i_map, Mario& i_mario);

	sf::FloatRect get_hit_box() const;
};
