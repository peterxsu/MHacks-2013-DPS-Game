#include <windows.h>
#include <fstream>
#include <ctime>
#include <vector>
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

using namespace std;

bool wKeyDown = false;
bool aKeyDown = false;
bool sKeyDown = false;
bool dKeyDown = false;
bool upKeyDown = false;
bool leftKeyDown = false;
bool downKeyDown = false;
bool rightKeyDown = false;

DWORD WINAPI backendMain(LPVOID lpParam)
{
	//create player, enemy, bullets
	Player p(340, 240, 100, 1, 1000);
	vector<Enemy> enemies;
	vector<Bullet*> bullets;
	bool tutorial = true;
	//create the two enemies that will be a part of the tutorial
	Enemy tutor1(200, 120, 5, 0);
	Enemy tutor2(480, 360, 5, 0);
	enemies.push_back(tutor1);
	enemies.push_back(tutor2);
	//edit the .sav file
	ofstream save;
	save.open("data.sav");
	for (int x = 0; x < 10; ++x)
	{
		for (int y = 0; y < 70; ++y)
			save << '0';
		save << '\n';
	}
	for (int x = 0; x < 30; ++x)
	{
		for (int y = 0; y < 10; ++y)
			save << '0';
		for (int y = 0; y < 50; ++y)
		{
			if ((x == 14 || x == 15) && (y == 24 || y == 25))
				save << 'P';
			else
				save << '1';
		}
		for (int y = 0; y < 10; ++y)
			save << '0';
		save << '\n';
	}
	for (int x = 0; x < 10; ++x)
	{
		for (int y = 0; y < 70; ++y)
			save << '0';
		save << '\n';
	}
	//detect keyboard presses and do appropriate action
	//automatically move enemies and bullets
	time_t prevTime = time(NULL), prevTime2 = prevTime, prevTime3 = prevTime, prevTime4 = prevTime;
	while (true)
	{
		//detect keyboard presses
		if (difftime(time(NULL), prevTime) > 0.1)
		{
			prevTime = time(NULL);
			for (int x = 0; x < bullets.size(); ++x)
				bullets[x]->move;
			for (int x = 0; x < enemies.size(); ++x)
				enemies[x].move(p.getX, p.getY);
			if (wKeyDown)
				p.move(0);
			else if (dKeyDown)
				p.move(1);
			else if (sKeyDown)
				p.move(2);
			else if (aKeyDown)
				p.move(3);
		}
		if (difftime(time(NULL), prevTime2) > 1)
		{
			prevTime2 = time(NULL);
			for (int x = 0; x < enemies.size(); ++x)
				if (enemies[x].getStrength())
					bullets.push_back(enemies[x].attack(p.getX, p.getY));
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
		if (difftime(time(NULL), prevTime4) > 5 && !tutorial)
		{
			prevTime4 = time(NULL);
			Enemy newEnemy(0,0,5,1);
			enemies.push_back(newEnemy);
		}
	}
}