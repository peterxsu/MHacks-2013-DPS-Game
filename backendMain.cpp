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
#include "EasyBMP.h"
#include "Display.h"

using namespace std;

bool wKeyDown = false;
bool aKeyDown = false;
bool sKeyDown = false;
bool dKeyDown = false;
bool upKeyDown = false;
bool leftKeyDown = false;
bool downKeyDown = false;
bool rightKeyDown = false;
Display display(250, 150);
Player p(340, 240, 100, 1, 1000);
vector<Enemy> enemies;
vector<Bullet*> bullets;
bool tutorial = true;
fstream save;
time_t prevTime, prevTime2, prevTime3, prevTime4;

void initialize();
void endTutorial(fstream&);
void gameActions();
string expandMapUp(fstream&);
string expandMapRight(fstream&);
string expandMapDown(fstream&);
string expandMapLeft(fstream&);
char addAdditions(char, int&);
void addMapUp(fstream&, string);
void addMapRight(fstream&, string);
void addMapDown(fstream&, string);
void addMapLeft(fstream&, string);

DWORD WINAPI backendMain(LPVOID lpParam)
{
	//detect keyboard presses and do appropriate action
	//automatically move enemies and bullets
	
	/*save.close();
	save.open("data.sav", fstream::in | fstream::app);
	for (int x = 0; x < 10; ++x)
	{
		addMapUp(save, expandMapUp(save));
		addMapDown(save, expandMapDown(save));
		addMapLeft(save, expandMapLeft(save));
		addMapRight(save, expandMapRight(save));
	}
	display.updateDisplay(enemies, bullets);
	save.close();*/
	return 0;
}

void initialize()
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
	display.updateDisplay(enemies, bullets);
}

void gameActions()
{
	if (difftime(time(NULL), prevTime) > 0.1)
	{
		prevTime = time(NULL);
		for (int x = 0; x < (int) bullets.size(); ++x)
			bullets[x]->move();
		for (int x = 0; x < (int) enemies.size(); ++x)
			enemies[x].move(p.getX(), p.getY());
		if (wKeyDown)
		{
			display.moveDisplay(0, 1);
			for (int x = 0; x < (int) bullets.size(); ++x)
				bullets[x]->move(0, 1);
			for (int x = 0; x < (int) enemies.size(); ++x)
				enemies[x].move(0, 1, 0);
				
		}
		else if (dKeyDown)
		{
			display.moveDisplay(-1, 0);
			for (int x = 0; x < (int) bullets.size(); ++x)
				bullets[x]->move(-1, 0);
			for (int x = 0; x < (int) enemies.size(); ++x)
				enemies[x].move(-1, 0, 0);
		}
		else if (sKeyDown)
		{
			display.moveDisplay(0, -1);
			for (int x = 0; x < (int) bullets.size(); ++x)
				bullets[x]->move(0, -1);
			for (int x = 0; x < (int) enemies.size(); ++x)
				enemies[x].move(0, -1, 0);
		}
		else if (aKeyDown)
		{
			display.moveDisplay(1, 0);
			for (int x = 0; x < (int) bullets.size(); ++x)
				bullets[x]->move(1, 0);
			for (int x = 0; x < (int) enemies.size(); ++x)
				enemies[x].move(1, 0, 0);
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
		int randonNum = rand() % 4;
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

void endTutorial()
{
	save.close();
	save.open("data.sav", fstream::out | fstream::trunc);
	for (int x = 0; x < 80; ++x)
	{
		for (int y = 0; y < 120; ++y)
		{
			if ((x == 39 || x == 40) && (y == 59 || y == 60))
				save << 'P';
			else
				save << '1';
		}
		save << '\n';
	}
}

string expandMapUp(fstream& fs)
{
	fs.close();
	fs.open("data.sav", fstream::in | fstream::app);
	string additions = "", nextLine;
	char below;
	int count = 0;
	getline(fs, nextLine);
	stringstream ss(nextLine);
	while (ss >> below)
		additions += addAdditions(below, count);
	return (additions + '\n');
}

string expandMapRight(fstream& fs)
{
	fs.close();
	fs.open("data.sav", fstream::in | fstream::app);
	string additions = "", nextLine;
	char left;
	int count = 0;
	while (getline(fs, nextLine))
	{
		left = nextLine[nextLine.size() - 2];
		additions += addAdditions(left, count);
		additions += '\n';
	}
	return additions;
}

string expandMapDown(fstream& fs)
{
	fs.close();
	fs.open("data.sav", fstream::in | fstream::app);
	string additions = "", nextLine;
	char above;
	int count = 0;
	stringstream* ss = nullptr;
	while (getline(fs, nextLine))
	{
		delete ss;
		ss = new stringstream(nextLine);
	}
	while (*ss >> above)
		additions += addAdditions(above, count);
	delete ss;
	return (additions + '\n');
}

string expandMapLeft(fstream& fs)
{
	fs.close();
	fs.open("data.sav", fstream::in | fstream::app);
	string additions = "", nextLine;
	char right;
	int count = 0;
	while (getline(fs, nextLine))
	{
		right = nextLine[0];
		additions += addAdditions(right, count);
		additions += '\n';
	}
	return additions;
}

char addAdditions(char inside, int& count)
{
	int tmp = rand() % 3;
	char outside;
	if (!tmp)
	{
		outside = '1';
		count = (count + 1) % 3;
	}
	else
	{
		if (count > 0 && count < 3)
		{
			outside = '1';
			count = (count + 1) % 3;
		}
		else if (inside == '1')
		{
			tmp = rand() % 2;
			if (tmp)
			{
				outside = '1';
				count = (count + 1) % 3;
			}
			else
			{
				outside = '0';
				count = 0;
			}
		}
		else
		{
			outside = '0';
			count = 0;
		}
	}
	return outside;
}

void addMapUp(fstream& fs, string additions)
{
	fs.close();
	fs.open("data.sav", fstream::in | fstream::app);
	if (!fs.is_open())
		exit(1);//throw exception
	string nextLine, newFile = additions;
	while (getline(fs, nextLine))
		newFile += nextLine + '\n';
	fs.close();
	fs.open("data.sav", fstream::out | fstream::trunc);
	fs << newFile;
}

void addMapRight(fstream& fs, string additions)
{
	fs.close();
	fs.open("data.sav", fstream::in | fstream::app);
	if (!fs.is_open())
		exit(1);//throw exception
	string nextLine, newFile = "";
	size_t pos = 0;
	while (getline(fs, nextLine))
	{
		newFile += nextLine;
		newFile += additions.substr(pos, additions.find('\n', pos) - pos + 1);
		pos = additions.find('\n', pos) + 1;
	}
	fs.close();
	fs.open("data.sav", fstream::out | fstream::trunc);
	fs << newFile;
}

void addMapDown(fstream& fs, string additions)
{
	fs.close();
	fs.open("data.sav", fstream::in | fstream::app);
	if (!fs.is_open())
		exit(1);//throw exception
	string nextLine, newFile = "";
	while (getline(fs, nextLine))
		newFile += nextLine + '\n';
	newFile += additions;
	fs.close();
	fs.open("data.sav", fstream::out | fstream::trunc);
	fs << newFile;
}

void addMapLeft(fstream& fs, string additions)
{
	fs.close();
	fs.open("data.sav", fstream::in | fstream::app);
	if (!fs.is_open())
		exit(1);//throw exception
	string nextLine, newFile = "";
	size_t pos = 0;
	while (getline(fs, nextLine))
	{
		newFile += additions.substr(pos, additions.find('\n', pos) - pos);
		pos = additions.find('\n', pos) + 1;
		newFile += nextLine + '\n';
	}
	fs.close();
	fs.open("data.sav", fstream::out | fstream::trunc);
	fs << newFile;
}

