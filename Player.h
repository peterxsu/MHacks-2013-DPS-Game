#include "Bullet.h"

#ifndef PLAYER_H
#define PLAYER_H
class Player
{
private:
	int x, y;
	int health;
	int strength;
	int cooldown; //cooldown time between shots in milliseconds

public:
	Player();
	Player(int, int, int, int, int);
	~Player();
	int getStrength() {return strength;}
	int getCooldown() {return cooldown;}
	int getHealth() {return health;}
	int getX() {return x;}
	int getY() {return y;}
	void takeHit(int damage) {health -= damage;}
	void move(int);
	Bullet* attack(int);
};
#endif