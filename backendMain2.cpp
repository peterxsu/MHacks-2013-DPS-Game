#ifndef BACKENDMAIN2_H
#define BACKENDMAIN2_H

#include <fstream>
#include <ctime>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "EasyBMP.h"
#include "Display.h"

using namespace std;

extern bool wKeyDown = false;
extern bool aKeyDown = false;
extern bool sKeyDown = false;
extern bool dKeyDown = false;
extern bool upKeyDown = false;
extern bool leftKeyDown = false;
extern bool downKeyDown = false;
extern bool rightKeyDown = false;
Display display(250, 150);
Player p(340, 240, 100, 1, 1000);
vector<Enemy> enemies;
vector<Bullet*> bullets;
bool tutorial = true;
fstream save;
time_t prevTime, prevTime2, prevTime3, prevTime4, prevTime5;

void endTutorial();
string expandMapUp();
string expandMapRight();
string expandMapDown();
string expandMapLeft();
char addAdditions(char, int&);
void addMapUp(string);
void addMapRight(string);
void addMapDown(string);
void addMapLeft(string);

DWORD WINAPI backendMain2(LPVOID lpParam)
{
	srand(time(NULL));
	//create the two enemies that will be a part of the tutorial
	Enemy tutor1(200, 120, 5, 0);
	Enemy tutor2(480, 360, 5, 0);
	enemies.push_back(tutor1);
	enemies.push_back(tutor2);
	//edit the .sav file
	save.open("data.sav", fstream::out | fstream::trunc);
	if (!save.is_open())
		exit(1);//probably should throw an exception
	for (int x = 0; x < 25; ++x)
	{
		for (int y = 0; y < 120; ++y)
			save << '0';
		save << '\n';
	}
	for (int x = 0; x < 30; ++x)
	{
		for (int y = 0; y < 35; ++y)
			save << '0';
		for (int y = 0; y < 50; ++y)
		{
			if ((x == 14 || x == 15) && (y == 24 || y == 25))
				save << 'P';
			else
				save << '1';
		}
		for (int y = 0; y < 35; ++y)
			save << '0';
		save << '\n';
	}
	for (int x = 0; x < 25; ++x)
	{
		for (int y = 0; y < 120; ++y)
			save << '0';
		save << '\n';
	}
	prevTime = time(NULL);
	prevTime2 = prevTime;
	prevTime3 = prevTime;
	prevTime4 = prevTime;
	prevTime5 = prevTime;
	display.updateDisplay(enemies, bullets);
	while (true)
	{
		if (difftime(time(NULL), prevTime) > 0.1)
		{
			prevTime = time(NULL);
			for (int x = 0; x < (int) bullets.size(); ++x)
				bullets[x]->move();
			for (int x = 0; x < (int) enemies.size(); ++x)
				enemies[x].move(p.getX(), p.getY());
		}
		if (difftime(time(NULL), prevTime5) > 0.05)
		{
			prevTime5 = time(NULL);
			if (wKeyDown)
			{
				display.moveDisplay(0, -1);
				for (int x = 0; x < (int) bullets.size(); ++x)
					bullets[x]->move(0, 1);
				for (int x = 0; x < (int) enemies.size(); ++x)
					enemies[x].move(0, 1, 0);
				if (p.getY() + display.getY() < 300)
					addMapUp(expandMapUp());
			}
			else if (dKeyDown)
			{
				display.moveDisplay(1, 0);
				for (int x = 0; x < (int) bullets.size(); ++x)
					bullets[x]->move(-1, 0);
				for (int x = 0; x < (int) enemies.size(); ++x)
					enemies[x].move(-1, 0, 0);
				if ((700 - p.getX()) + (display.getMapX() - (700 + display.getX())) > 400)
					addMapRight(expandMapRight());
			}
			else if (sKeyDown)
			{
				display.moveDisplay(0, 1);
				for (int x = 0; x < (int) bullets.size(); ++x)
					bullets[x]->move(0, -1);
				for (int x = 0; x < (int) enemies.size(); ++x)
					enemies[x].move(0, -1, 0);
				if ((500 - p.getY()) + (display.getMapY() - (500 + display.getY())) > 300)
					addMapDown(expandMapDown());
			}
			else if (aKeyDown)
			{
				display.moveDisplay(-1, 0);
				for (int x = 0; x < (int) bullets.size(); ++x)
					bullets[x]->move(1, 0);
				for (int x = 0; x < (int) enemies.size(); ++x)
					enemies[x].move(1, 0, 0);
				if (p.getX() + display.getX() < 400)
					addMapLeft(expandMapLeft());
			}
		}
		if (difftime(time(NULL), prevTime2) > 1)
		{
			prevTime2 = time(NULL);
			for (int x = 0; x < (int) enemies.size(); ++x)
				if (enemies[x].getStrength())
					bullets.push_back(enemies[x].attack(p.getX(), p.getY()));
		}
		if (difftime(time(NULL), prevTime3) > 0.001*p.getCooldown())
		{
			prevTime3 = time(NULL);
			if (upKeyDown)
				bullets.push_back(p.attack(0));
			else if (rightKeyDown)
				bullets.push_back(p.attack(1));
			else if (downKeyDown)
				bullets.push_back(p.attack(2));
			else if (leftKeyDown)
				bullets.push_back(p.attack(3));
		}
		//should only happen when the tutorial is done so determine when the two tutorial enemies are destroyed
		if (!tutorial && difftime(time(NULL), prevTime4) > 5)
		{
			int randomNum = rand() % 4;
			prevTime4 = time(NULL);
			int enemyX, enemyY;
			if (randomNum == 0)
			{
				randomNum = rand() % 70;
				enemyX = randomNum;
				enemyY = 0;
			}
			else if (randomNum == 1)
			{
				randomNum = rand() % 50;
				enemyX = 69;
				enemyY = randomNum;
			}
			else if (randomNum == 2)
			{
				randomNum = rand() % 70;
				enemyX = randomNum;
				enemyY = 49;
			}
			else
			{
				randomNum = rand() % 50;
				enemyX = 0;
				enemyY = randomNum;
			}
			Enemy newEnemy(enemyX*10,enemyY*10,5,1);
			enemies.push_back(newEnemy);
		}
		if (enemies.size() == 0 && tutorial)
		{
			endTutorial();
			tutorial = false;
			prevTime4 = time(NULL);
		}
		display.updateDisplay(enemies, bullets);
	}
}

#endif