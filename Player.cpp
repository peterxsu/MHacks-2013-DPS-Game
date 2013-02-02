#include "Player.h"


Player::Player()
{
	x = 0;
	y = 0;
	health = 1;
	attack = 1;
	direction = 0;
	cooldown = 1000;
}

Player::Player(int xpos, int ypos, int h, int a, int d, int c)
{
	x = xpos;
	y = ypos;
	health = h;
	attack = a;
	direction = d;
	cooldown = c;
}

Player::~Player()
{

}

void Player::takeHit(int damage)
{
	health -= damage;
}

void Player::move(int dx, int dy)
{
	x += dx;
	y += dy;
}

void Player::turn(int d)
{
	direction = d;
}
