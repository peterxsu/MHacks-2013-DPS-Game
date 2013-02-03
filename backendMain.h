#ifndef BACKENDMAIN_H
#define BACKENDMAIN_H

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

#endif