#include "Player.h"
#include "Bullet.h"


Player::Player()
{
	x = 0;
	y = 0;
	health = 1;
	strength = 1;
	cooldown = 1000;
}

Player::Player(int xpos, int ypos, int h, int s, int c)
{
	x = xpos;
	y = ypos;
	health = h;
	strength = s;
	cooldown = c;
}

Player::~Player()
{

}

void Player::move(int dir)
{
	if (dir%2)
		x -= dir-2;
	else
		y += dir-1;
}

Bullet* Player::attack(int dir)
{
	Bullet* b = new Bullet(x, y, strength, dir);
	b->move();
	return b;
}