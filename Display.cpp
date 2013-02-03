#include "Display.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

Display::Display()
{
	x = 0;
	y = 0;
	image.SetSize(700, 500);
	image.SetBitDepth(32);
	black.ReadFromFile("black.bmp");
	white.ReadFromFile("white.bmp");
	person.ReadFromFile("person.bmp");
	enemy.ReadFromFile("enemy.bmp");
	bullet.ReadFromFile("bullet.bmp");
	world.open("data.sav");
	image.WriteToFile("display.bmp");
}

Display::Display(int xpos, int ypos)
{
	x = xpos;
	y = ypos;
	image.SetSize(700, 500);
	image.SetBitDepth(32);
	black.ReadFromFile("black.bmp");
	white.ReadFromFile("white.bmp");
	person.ReadFromFile("person.bmp");
	enemy.ReadFromFile("enemy.bmp");
	bullet.ReadFromFile("bullet.bmp");
	world.open("data.sav");
	image.WriteToFile("display.bmp");
}

Display::~Display()
{

}

void Display::updateDisplay(vector<Enemy>& enemies, vector<Bullet*>& bullets)
{
	string tmp;
	world.close();
	world.open("data.sav");
	int yOffset = y%10;
	int xOffset = x%10;
	for (long long i = 0; i < (y/10); i++)
		getline(world, tmp);
	//draw the background environment
	for (int i = 0; i < (50 + (yOffset+9)/10); i++)
	{
		getline(world, tmp);
		for (int j = 0; j < (70 + (xOffset +9)/10); j++)
		{
			if (tmp[x/10 + j] == '0')
				RangedPixelToPixelCopy(black, 0, 9, 9, 0, image, j*10-xOffset, i*10-yOffset);
			else if (tmp[x/10 + j] == '1')
				RangedPixelToPixelCopy(white, 0, 9, 9, 0, image, j*10-xOffset, i*10-yOffset);
		}
	}
	//draw the Player, Enemies, and Bullets
	RangedPixelToPixelCopy(person, 0, 19, 19, 0, image, 340, 240);
	for (int i = 0; i < enemies.size(); i++)
		RangedPixelToPixelCopy(enemy, 0, 9, 0, 9, image, enemies[i].getX(), enemies[i].getY());
	for (int i = 0; i < bullets.size(); i++)
		RangedPixelToPixelCopy(bullet, 0, 1, 0, 1, image, bullets[i]->getX(), bullets[i]->getY());
	image.WriteToFile("display.bmp");
}

void Display::moveDisplay(int dx, int dy)
{
	x += dx;
	y += dy;
}

long long Display::getMapX()
{
	string nextLine;
	getline(world, nextLine);
	return (long long) nextLine.size() * 10;
}

long long Display::getMapY()
{
	string nextLine;
	long long count = 0;
	while (getline(world, nextLine))
		count += 10;
	return count;
}