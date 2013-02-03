#ifndef BACKENDMAIN_H
#define BACKENDMAIN_H

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

void endTutorial();
string expandMapUp(fstream&);
string expandMapRight(fstream&);
string expandMapDown(fstream&);
string expandMapLeft(fstream&);
char addAdditions(char, int&);
void addMapUp(fstream&, string);
void addMapRight(fstream&, string);
void addMapDown(fstream&, string);
void addMapLeft(fstream&, string);

#endif