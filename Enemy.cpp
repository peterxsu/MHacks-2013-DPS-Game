#include "Enemy.h"
#include <cmath>

Enemy::Enemy(int x = 0, int y = 0, int h = 1, int s = 1)
{
	this->x = x;
	this->y = y;
	health = h;
	strength = s;
}

Enemy::~Enemy()
{

}

void Enemy::move(int playerx, int playery)
{
	int diffx = playerx - x, diffy = playery - y;
	if (diffx >= diffy)
		x += diffx/abs(diffx);
	else
		y += diffy/abs(diffy);
}

Bullet* Enemy::attack(int playerx, int playery)
{
	int diffx = playerx - x, diffy = playery - y;
	Bullet* attBullet = nullptr;
	if (diffx <= diffy)
		attBullet = new Bullet(x, y, strength, 2 - diffx/abs(diffx));//create bullet that goes in the sideways direction
	else
		attBullet = new Bullet(x, y, strength, 1 + diffy/abs(diffy));//create bullet that goes in the up/down direction
	return attBullet;
}

void Enemy::takeHit(int damage)
{
	health -= damage;
}