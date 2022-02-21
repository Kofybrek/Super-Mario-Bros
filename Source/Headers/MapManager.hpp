#pragma once

class MapManager
{
	std::vector<Object> brick_particles;
	std::vector<Object> question_block_coins;

	sf::Image map_sketch;

	sf::Sprite cell_sprite;

	sf::Texture map_texture;

	Animation coin_animation;
	Animation question_block_animation;

	Map map;
public:
	MapManager();

	unsigned short get_map_sketch_height() const;
	unsigned short get_map_sketch_width() const;
	unsigned short get_map_width() const;

	void add_brick_particles(const unsigned short i_x, const unsigned short i_y);
	void add_question_block_coin(const unsigned short i_x, const unsigned short i_y);
	void draw_map(const bool i_draw_background, const bool i_underground, const unsigned i_view_x, sf::RenderWindow& i_window);
	void set_map_cell(const unsigned short i_x, const unsigned short i_y, const Cell& i_cell);
	void set_map_size(const unsigned short i_new_size);
	void update();
	void update_map_sketch(const unsigned char i_current_level);

	std::vector<unsigned char> map_collision(const std::vector<Cell>& i_check_cells, const sf::FloatRect& i_hitbox) const;
	std::vector<unsigned char> map_collision(const std::vector<Cell>& i_check_cells, std::vector<sf::Vector2i>& i_collision_cells, const sf::FloatRect& i_hitbox) const;

	sf::Color get_map_sketch_pixel(const unsigned short i_x, const unsigned short i_y) const;
};