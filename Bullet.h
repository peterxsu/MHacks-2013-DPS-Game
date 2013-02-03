#ifndef BULLET_H
#define BULLET_H
class Bullet
{
private:
	int x, y;
	int strength;
	int direction;

public:
	Bullet();
	Bullet(int, int, int, int);
	~Bullet();
	int getStrength() {return strength;}
	int getX() {return x;}
	int getY() {return y;}
	void move();
	void move(int, int);
};
#endif