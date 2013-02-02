#include "Bullet.h"

#ifndef ENEMY_H
#define ENEMY_H
class Enemy
{
	int x, y, health, strength;

public:
	Enemy(int, int, int, int);
	~Enemy();
	void move(int, int);
	Bullet* attack(int, int);
	void takeHit(int);
	int getStrength() {return strength;}
	int getX() {return x;}
	int getY() {return y;}
};
#endif