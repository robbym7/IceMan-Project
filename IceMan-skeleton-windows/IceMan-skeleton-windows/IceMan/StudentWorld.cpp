#include "StudentWorld.h"
#include <string>

#include "Actor.h"
#include <cmath>
#include <vector>
using namespace std;


GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

int StudentWorld::init() {
	if (getLives() > 0) {
		createOilField();

		return GWSTATUS_CONTINUE_GAME;
	}
	else {

		return GWSTATUS_PLAYER_DIED;
	}

}

int StudentWorld::move() {
	//return GWSTATUS_PLAYER_DIED;
	updateDisplayText();

	//player needs to move first 
	player->move();
	//check if each actor is alive
	for (auto a : actors) {
		if (a->isAlive()) {
			//actors perform their action then check if player died or finished level
			a->move();

		}
	}
	//remove any actors that died
	removeDeadActors();

	if (playerDiedDuringThisTick()) {
		return GWSTATUS_PLAYER_DIED;
	}
	else {

		if (playerCompletedCurrentLevel()) {

			playSound(GWSTATUS_FINISHED_LEVEL);
			return GWSTATUS_FINISHED_LEVEL;
		}
		//if player hasnt died or finished the level then continue the game
		return GWSTATUS_CONTINUE_GAME;
	}

}

//is called when player loses a life
void StudentWorld::cleanUp() {

	for (auto a : actors) {
		delete a;
	}
	//actors.clear memory leaks if you dont delete one by one first
	actors.clear();

	//need to delete player since stored seperate from actors
	if (player != nullptr) {
		delete player;
		player = nullptr;
	}

	//need to delete the ice array seperatley since it is stored as a seperate 2D array
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 60; j++) {
			delete icefield[i][j];
			icefield[i][j] = nullptr;
		}
	}
	return;
}
//destructor
StudentWorld::~StudentWorld() { cleanUp(); }



void StudentWorld::addActor(Actor* a) {
	actors.push_back(a);
	return;
}

void StudentWorld::removeDeadActors() {

	for (auto a : actors) {
		if (a->isAlive() == false) {
			a->setVisible(false);
		}
	}
	return;
}

//TODO: commented ones rely on items being done(names can change)
//example:     Lvl: 52 Lives: 3 Hlth: 80% Wtr: 20 Gld: 3 Oil Left: 2 Sonar: 1 Scr: 321000
void StudentWorld::updateDisplayText() {
  int level = getCurrentGameLevel();
    int lives = getNumLivesLeft();
    int health = getCurrentHealth();
    int squirts = getSquirtsLeftInSquirtGun();
    int gold = getPlayerGoldCount();
    int barrelsLeft = getNumberOfBarrelsRemainingToBePickedUp();
    int sonar = getPlayerSonarChargeCount();
    int score = getCurrentScore();
	//need to make all of this into a string then format
	string s = formatDisplayTest(level, lives, health, squirts, gold, barrelsLeft, sonar, score);
	//prints to the stats in game
	setGameStatText(s);

	return;
}

//should have a 2D array of ice actors
//should place the IceMan 
void StudentWorld::createOilField() {
	//srand(time(NULL));
	player = new IceMan(this, 30, 60);
	
	//currentLevel = getLevel();
	remainingOil = 9;
	currentLevel = getLevel();
    numBoulders = min((currentLevel / 2) + 2, 9);
    numGold = max(5 - (currentLevel / 2), 2);
    numOilBarrels = min(2 + currentLevel, 21);
    remainingOil = numOilBarrels;
    generateBoulders(numBoulders);

	// test creating new Ice blocks
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 60; j++) {
			if (i >= 30 && i <= 33 && j > 3) { 
				icefield[i][j] = nullptr; 
			}
			else {
				icefield[i][j] = new Ice(this, i, j);
			}
		}
	}
	addActor((new RegularProtester(this, 60, 60, IID_PROTESTER)));
	//actors.push_back(new HardcoreProtester(this, 60, 60, IID_HARD_CORE_PROTESTER));

}
void StudentWorld::generateBoulders(int totalBoulders)
{
    int xCord;
    int yCord;
    int counter = 0;
    while (counter < totalBoulders) {
        xCord = rand() % 60;
        yCord = rand() % 34 + 20;
        if (xCord >= 27 && xCord <= 33) {
            continue;
        }
        if (doesOverlap(xCord, yCord, 6.0)) {
            continue;
        }
        addActor(new Boulder(this, xCord, yCord));
            counter++;
    }
}
bool StudentWorld::doesOverlap(int x, int y, double radius)
{
    double dist;
    for (auto a : actors) {
        int actorX = a->getX();
        int actorY = a->getY();
        dist = distance(actorX, actorY, x, y);
        if (dist <= 6.0) {
            return true;
        }
    }
    return false;
}

int StudentWorld::getRemainingOil() const {
	return remainingOil;
}

bool StudentWorld::playerCompletedCurrentLevel() const {
	if (remainingOil <= 0) {
		return true;
	}
	else {
		return false;
	}
}

bool StudentWorld::playerDiedDuringThisTick() {
	if (player->isAlive() == true) {
		return false;
	}
	else {
		decLives();
		return true;
	}

	return false;
}
double StudentWorld::distance(int x1, int y1, int x2, int y2) const
{
	return sqrt((pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0));
}

bool StudentWorld::isNearIceMan(Actor* a, int radius) const
{
	int actorX = a->getX();
	int actorY = a->getY();
	int playerX = player->getX();
	int playerY = player->getY();
	double dist = distance(actorX, actorY, playerX, playerY);
	if (dist <= radius)
		return true;
	else
		return false;
}


void StudentWorld::playerDig(GraphObject::Direction dir, int x, int y) {
	switch (dir) {
	case GraphObject::right:
		if (x + 3 >= 64 || y + 3 >= 60)
			return;

		for (int i = 0; i < 4; i++) {
			if (icefield[x + 3][y + i] != nullptr) {
				playSound(SOUND_DIG);
				digIce(x+3 , y + i);
			}
		}
		break;

	case(GraphObject::left):
		if (x < 0 || y + 3 >= 60)
			return;
		for (int i = 0; i < 4; i++) {
			if (icefield[x][y + i] != nullptr) {
				playSound(SOUND_DIG);
				digIce(x, y + i);
			}
		}
		break;

	case(GraphObject::down):
		if (y < 0 || x + 3 >= 64)
			return;
		for (int i = 0; i < 4; i++) {
			if (icefield[x + i][y] != nullptr) {
				playSound(SOUND_DIG);
				digIce(x + i, y);
			}
		}
		break;

	case(GraphObject::up):
		if (y >= 60 || x + 3 >= 64)
			return;
		for (int i = 0; i < 4; i++) {
			if (icefield[x + i][y] != nullptr) {
				playSound(SOUND_DIG);
				digIce(x + i, y);
			}
		}
		break;
	}
}


void StudentWorld::digIce(int x, int y) {
	if (x <= 0 || x >= 64 || y <= 0 || y >= 60) { 
		return; 
	}
	if (icefield[x][y] == nullptr) {
		return;
	}

	icefield[x][y]->setVisible(false);
	delete icefield[x][y];
	icefield[x][y] = nullptr;
}

bool StudentWorld::canMove(int x, int y) {
	return (x >= 0 && x < 64 && y >= 0 && y < 64);
}

std::string StudentWorld::formatDisplayTest(int level, int lives, int health, int squirts, int gold, int barrelsLeft, int sonar, int score)
{
    // Lvl: 52 Lives: 3 Hlth: 80% Wtr: 20 Gld: 3 Oil Left: 2 Sonar: 1 Scr: 32100
    string text = "Lvl: ";
    if (level < 10) 
        text += " ";
    text += to_string(level) + "  ";
    text += "Lives: " + to_string(lives) + "  ";
    text += "Hlth: ";
    if (health * 10 < 10) 
        text += " ";
    text += to_string(health * 10) + "%  ";
    text += "Wtr: ";
    if (squirts < 10)
        text += " "; 
    text += to_string(squirts) + "  ";
    text += "Gld: ";
    //NOT needed gold is max 5
    //if (gold < 10)
    //    text += " ";
    text += to_string(gold) + "  ";
    text += "Oil Left: ";
    if (barrelsLeft < 10)
        text += " ";
    text += to_string(barrelsLeft) + "  ";
    text += "Sonar: ";
    if (sonar < 10)
        text += " ";
    text += to_string(sonar) + "  ";
    string scoreStr = to_string(score);
    //add 0s to the front of score until its 6 digits
    while (scoreStr.length() < 6) {
        scoreStr = "0" + scoreStr;
    }
    text += "Scr: " + scoreStr;

    return text;
}

//getters
int StudentWorld::getNumLivesLeft() const
{
    return getLives();
}

int StudentWorld::getCurrentHealth() const
{
    return player->getHitPoints();
}

int StudentWorld::getSquirtsLeftInSquirtGun() const
{
    return player->getWater();
}

int StudentWorld::getPlayerGoldCount() const
{
    return player->getGold();
}

int StudentWorld::getNumberOfBarrelsRemainingToBePickedUp() const
{
    return remainingOil;
}

int StudentWorld::getPlayerSonarChargeCount() const
{
    return player->getSonar();
}

int StudentWorld::getCurrentScore() const
{
    return getScore();
}

bool StudentWorld::scanPlayer(Actor* a, int radius)
{
    if (isNearIceMan(a, radius))
        return true;
    return false;
}

IceMan* StudentWorld::getPlayer()
{
    return player;
}
/*
all GameWorld functions

unsigned int getLives() const;
void decLives();
void incLives();
unsigned int getScore() const;
unsigned int getLevel() const;
void increaseScore(unsigned int howMuch);
void setGameStatText(string text);
bool getKey(int& value);
void playSound(int soundID);

*/