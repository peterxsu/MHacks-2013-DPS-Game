#include "Player.h"


Player::Player()
{
	x = 0;
	y = 0;
	health = 1;
	strength = 1;
	direction = 0;
	cooldown = 1000;
}

Player::Player(int xpos, int ypos, int h, int s, int d, int c)
{
	x = xpos;
	y = ypos;
	health = h;
	strength = s;
	direction = d;
	cooldown = c;
}

Player::~Player()
{

}

void Player::move(int dx, int dy)
{
	x += dx;
	y += dy;
}
