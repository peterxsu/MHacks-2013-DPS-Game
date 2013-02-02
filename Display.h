#include "EasyBMP.h"
#include "Enemy.h"
#include "Bullet.h"
#include <vector>
#include <iostream>
#include <fstream>

#ifndef DISPLAY_H
#define DISPLAY_H
class Display
{
private:
	long long x, y;
	BMP image;
	BMP black, white, person, enemy, bullet;
	ifstream world;
public:
	Display();
	Display(int, int);
	~Display();
	void updateDisplay(vector<Enemy>&, vector<Bullet>&);
};
#endif