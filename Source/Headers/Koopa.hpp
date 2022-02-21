#pragma once

class Koopa : public Enemy
{
	//We'll use this to make sure that we check the collision with Mario only once when he collides with us.
	bool check_collision;
	bool flipped;
	//Look at the declaration of the Goomba class. I don't wanna explain this again.
	//I know I can just copy and paste, but I want you to do some work too, y'know.
	//I mean, why do I have to do everything for you?
	//Why do I have to waste my time copying and pasting 4 lines of comments when you can just open the file I told you to open?
	//Are you better than me?
	bool no_collision_dying;
	bool underground;

	//0 - Koopa walk
	//1 - Koopa shell
	//2 - Koopa shell slide
	unsigned char state;

	unsigned short get_out_timer;

	sf::Sprite sprite;

	sf::Texture texture;

	Animation get_out_animation;
	Animation walk_animation;
public:
	Koopa(const bool i_underground, const float i_x, const float i_y);

	bool get_dead(const bool i_deletion) const;

	void die(const unsigned char i_death_type);
	void draw(const unsigned i_view_x, sf::RenderWindow& i_window);
	void update(const unsigned i_view_x, const std::vector<std::shared_ptr<Enemy>>& i_enemies, const MapManager& i_map_manager, Mario& i_mario);
};