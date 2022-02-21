#pragma once

class Mushroom
{
	bool dead;

	char horizontal_direction;

	float vertical_speed;
	float x;
	float y;

	//Before the mushroom begins to move, it needs to get out of the question block.
	unsigned short start_y;

	sf::Sprite sprite;

	sf::Texture texture;
public:
	Mushroom(const float i_x, const float i_y);

	bool get_dead() const;

	void draw(const unsigned i_view_x, sf::RenderWindow& i_window);
	void set_dead(const bool i_value);
	void update(const unsigned i_view_x, const MapManager& i_map_manager);

	sf::FloatRect get_hit_box() const;
};