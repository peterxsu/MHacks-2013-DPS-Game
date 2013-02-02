#include <windows.h>
#include <fstream>
#include <ctime>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>
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

string expandMapUp(fstream&);
string expandMapRight(fstream&);
string expandMapDown(fstream&);
string expandMapLeft(fstream&);

void addMapUp(fstream&, string);
void addMapRight(fstream&, string);
void addMapDown(fstream&, string);
void addMapLeft(fstream&, string);

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
	fstream save;
	save.open("data.sav");
	if (!save.is_open())
		;//probably should throw an exception
	for (int x = 0; x < 235; ++x)
	{
		for (int y = 0; y < 700; ++y)
			save << '0';
		save << '\n';
	}
	for (int x = 0; x < 30; ++x)
	{
		for (int y = 0; y < 325; ++y)
			save << '0';
		for (int y = 0; y < 50; ++y)
		{
			if ((x == 14 || x == 15) && (y == 24 || y == 25))
				save << 'P';
			else
				save << '1';
		}
		for (int y = 0; y < 325; ++y)
			save << '0';
		save << '\n';
	}
	for (int x = 0; x < 235; ++x)
	{
		for (int y = 0; y < 700; ++y)
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
				bullets[x]->move();
			for (int x = 0; x < enemies.size(); ++x)
				enemies[x].move(p.getX(), p.getY());
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
		if (difftime(time(NULL), prevTime4) > 5 && !tutorial)
		{
			prevTime4 = time(NULL);
			Enemy newEnemy(0,0,5,1);
			enemies.push_back(newEnemy);
		}
	}
}

string expandMapUp(fstream& fs)
{
	srand(time(NULL));
	string additions = "";
	char below, above;
	int blocking = -1;
	bool blocked = true;
	while (fs >> below && below != '\n')
	{
		int obstacle = rand() % 5;
		if (obstacle == 0)
		{
			above = '0';
			if (blocking == -1 && below == '1')
				blocking = 0;
			else if (blocking > 0 && below == '1')
			{
				if (blocking == 3)
					blocked = false;
				blocking = 0;
			}
			else if (blocking >= 0 && below == '0')
			{
				if (blocking == 3 || !blocked)
				{
					blocking = -1;
					blocked = true;
				}
				else
					for (int x = 0; x < 3; ++x)//figure out a way to correct the last few entries to provide enough spaces
						additions[additions.size() - 1 - x] = '1';
			}
		}
		else
		{
			above = '1';
			if (blocking >= 0 && below == '1')
			{
				if (blocking == 3)
					blocked = false;
				else
					blocking++;
			}
			else if (below == '1')
				blocking = 1;
			else if (blocking >= 0 && below == '0')
			{
				if (blocking == 3 || !blocked)
				{
					blocked = true;
					blocking = -1;
				}
				else
					for (int x = 0; x < 3; ++x)//figure out a way to correct the last few entries to provide enough spaces
						additions[additions.size() - 1 - x] = '1';
			}
		}
		additions += above;
	}
	if (blocking < 3 && blocked)
		for (int x = 0; x < 3; ++x)
			additions[additions.size() - 1 - x] = '1';
	return (additions + '\n');
}

string expandMapRight(fstream& fs)
{
	srand(time(NULL));
	string additions = "", nextLine;
	char left, right;
	int blocking = -1;
	bool blocked = true;
	while (getline(fs, nextLine))
	{
		left = nextLine[nextLine.size() - 2];
		int obstacle = rand() % 5;
		if (obstacle == 0)
		{
			right = '0';
			if (blocking == -1 && left == '1')
				blocking = 0;
			else if (blocking > 0 && left == '1')
			{
				if (blocking == 3)
					blocked = false;
				blocking = 0;
			}
			else if (blocking >= 0 && left == '0')
			{
				if (blocking == 3 || !blocked)
				{
					blocking = -1;
					blocked = true;
				}
				else
					for (int x = 0 ; x < 3; ++x)//figure out a way to correct the last few entries to provide enough spaces
						additions[additions.size() - 2 - 2 * x] = '1';
			}
		}
		else
		{
			right = '1';
			if (blocking >= 0 && left == '1')
			{
				if (blocking == 3)
					blocked = false;
				else
					blocking++;
			}
			else if (left == '1')
				blocking = 1;
			else if (blocking >= 0 && left == '0')
			{
				if (blocking == 3 || !blocked)
				{
					blocked = true;
					blocking = -1;
				}
				else
					for (int x = 0 ; x < 3; ++x)//figure out a way to correct the last few entries to provide enough spaces
						additions[additions.size() - 2 - 2 * x] = '1';
			}
		}
		additions += right + '\n';
	}
	if (blocking < 3 && blocked)
		for (int x = 0; x < 3; ++x)
			additions[additions.size() - 2 - 2 * x] = '1';
	return additions;
}

string expandMapDown(fstream& fs)
{
	srand(time(NULL));
	string additions = "", nextLine;
	char below, above;
	int blocking = -1;
	bool blocked = true;
	while (getline(fs, nextLine)) {}
	stringstream ss(nextLine);
	while (ss >> above && above != '\n')
	{
		int obstacle = rand() % 5;
		if (obstacle == 0)
		{
			below = '0';
			if (blocking == -1 && above == '1')
				blocking = 0;
			else if (blocking > 0 && above == '1')
			{
				if (blocking == 3)
					blocked = false;
				blocking = 0;
			}
			else if (blocking >= 0 && above == '0')
			{
				if (blocking == 3 || !blocked)
				{
					blocking = -1;
					blocked = true;
				}
				else
					for (int x = 0; x < 3; ++x)//figure out a way to correct the last few entries to provide enough spaces
						additions[additions.size() - 1 - x] = '1';
			}
		}
		else
		{
			below = '1';
			if (blocking >= 0 && above == '1')
			{
				if (blocking == 3)
					blocked = false;
				else
					blocking++;
			}
			else if (above == '1')
				blocking = 1;
			else if (blocking >= 0 && above == '0')
			{
				if (blocking == 3 || !blocked)
				{
					blocked = true;
					blocking = -1;
				}
				else
					for (int x = 0; x < 3; ++x)//figure out a way to correct the last few entries to provide enough spaces
						additions[additions.size() - 1 - x] = '1';
			}
		}
		additions += below;
	}
	if (blocking < 3 && blocked)
		for (int x = 0; x < 3; ++x)
			additions[additions.size() - 1 - x] = '1';
	return (additions + '\n');
}

string expandMapLeft(fstream& fs)
{
	srand(time(NULL));
	string additions = "", nextLine;
	char left, right;
	int blocking = -1;
	bool blocked = true;
	while (getline(fs, nextLine))
	{
		right = nextLine[0];
		int obstacle = rand() % 5;
		if (obstacle == 0)
		{
			left = '0';
			if (blocking == -1 && right == '1')
				blocking = 0;
			else if (blocking > 0 && right == '1')
			{
				if (blocking == 3)
					blocked = false;
				blocking = 0;
			}
			else if (blocking >= 0 && right == '0')
			{
				if (blocking == 3 || !blocked)
				{
					blocking = -1;
					blocked = true;
				}
				else
					for (int x = 0 ; x < 3; ++x)//figure out a way to correct the last few entries to provide enough spaces
						additions[additions.size() - 2 - 2 * x] = '1';
			}
		}
		else
		{
			left = '1';
			if (blocking >= 0 && right == '1')
			{
				if (blocking == 3)
					blocked = false;
				else
					blocking++;
			}
			else if (right == '1')
				blocking = 1;
			else if (blocking >= 0 && right == '0')
			{
				if (blocking == 3 || !blocked)
				{
					blocked = true;
					blocking = -1;
				}
				else
					for (int x = 0 ; x < 3; ++x)//figure out a way to correct the last few entries to provide enough spaces
						additions[additions.size() - 2 - 2 * x] = '1';
			}
		}
		additions += left + '\n';
	}
	if (blocking < 3 && blocked)
		for (int x = 0; x < 3; ++x)
			additions[additions.size() - 2 - 2 * x] = '1';
	return additions;
}

void addMapUp(fstream& fs, string additions)
{
	if (!fs.is_open())
		;//throw exception
	string nextLine, newFile = additions;
	while (getline(fs, nextLine))
		newFile += nextLine + '\n';
	fs << newFile;
}

void addMapRight(fstream& fs, string additions)
{
	if (!fs.is_open())
		;//throw exception
	string nextLine, newFile = "";
	size_t pos = 0;
	while (getline(fs, nextLine))
	{
		newFile += nextLine;
		newFile += additions.substr(pos, additions.find('\n', pos) - pos + 1);
		pos = additions.find('\n', pos) + 1;
	}
	fs << newFile;
}

void addMapDown(fstream& fs, string additions)
{
	if (!fs.is_open())
		;//throw exception
	string nextLine, newFile = "";
	while (getline(fs, nextLine))
		newFile += nextLine + '\n';
	newFile += additions;
	fs << newFile;
}

void addMapLeft(fstream& fs, string additions)
{
	if (!fs.is_open())
		;//throw exception
	string nextLine, newFile = "";
	size_t pos = 0;
	while (getline(fs, nextLine))
	{
		newFile += additions.substr(pos, additions.find('\n', pos) - pos);
		pos = additions.find('\n', pos) + 1;
		newFile += nextLine + '\n';
	}
	fs << newFile;
}
