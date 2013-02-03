#include "Bullet.h"


Bullet::Bullet()
{
	x = 0;
	y = 0;
	strength = 0;
	direction = 0;
}

Bullet::Bullet(int xpos, int ypos, int s, int d)
{
	x = xpos;
	y = ypos;
	strength = s;
	direction = d;
}

Bullet::~Bullet()
{

}

void Bullet::move()
{
	if (direction%2)
		x -= direction-2;
	else
		y += direction-1;
}

void Bullet::move(int dx, int dy)
{
	x += dx;
	y += dy;
}
