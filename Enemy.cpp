#include "Enemy.h"
#include <cmath>

Enemy::Enemy(int x = 0, int y = 0, int h = 1, int a = 1)
{
	this->x = x;
	this->y = y;
	health = h;
	attack = a;
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

void Enemy::fire()
{
	//create bullet
}

void Enemy::takeHit(int damage)
{
	health -= damage;
}