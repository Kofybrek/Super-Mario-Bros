#pragma once

constexpr float GOOMBA_SPEED = 1;
constexpr float GRAVITY = 0.25f;
constexpr float MARIO_ACCELERATION = 0.25f;
//If you've ever played SMB, you know that when Mario dies, he stands still for a certain amount of time (this variable defines the duration) and then bounces off the screen.
constexpr float MARIO_DEATH_DURATION = 32;
constexpr float MARIO_JUMP_SPEED = -4;
constexpr float MARIO_WALK_SPEED = 2;
//If we remove this, Mario will be able to pass through walls, as the speed can exceed the size of the cell.
constexpr float MAX_VERTICAL_SPEED = 8;

constexpr unsigned char CELL_SIZE = 16;
//We want Mario to jump higher the longer we press the jump button. So we're gonna use this constant.
constexpr unsigned char MARIO_JUMP_TIMER = 8;
constexpr unsigned char SCREEN_RESIZE = 2;

//We're gonna update goombas ONLY if they're inside this area.
constexpr unsigned short ENTITY_UPDATE_AREA = 64;
//After this we'll delete the goomba.
constexpr unsigned short GOOMBA_DEATH_DURATION = 32;
//We'll update the frame once every [GOOMBA_WALK_ANIMATION_SPEED] frames.
constexpr unsigned short GOOMBA_WALK_ANIMATION_SPEED = 32;
constexpr unsigned short MARIO_WALK_ANIMATION_SPEED = 4;
constexpr unsigned short SCREEN_HEIGHT = 240;
constexpr unsigned short SCREEN_WIDTH = 256;

constexpr std::chrono::microseconds FRAME_DURATION(16667);

enum Cell
{
	Brick,
	Empty,
	Pipe,
	QuestionBlock,
	Wall
};

//I'm in love with typedef!
typedef std::vector<std::array<Cell, SCREEN_HEIGHT / CELL_SIZE>> Map;
