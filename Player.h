#pragma once
class Player
{
private:
	int x, y;
	int health;
	int attack;
	int direction; //0 = North, 1 = East, 2 = South, 3 = West
	int cooldown; //cooldown time between shots in milliseconds

public:
	Player();
	Player(int, int, int, int, int, int);
	~Player();
	void takeHit(int);
	void move(int, int);
	void turn(int);
};

