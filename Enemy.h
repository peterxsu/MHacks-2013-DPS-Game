#include "Bullet.h"

class Enemy
{
	int x, y, health, strength;

public:
	Enemy(int, int, int, int);
	~Enemy();
	void move(int, int);
	Bullet* attack(int, int);
	void takeHit(int);
};

