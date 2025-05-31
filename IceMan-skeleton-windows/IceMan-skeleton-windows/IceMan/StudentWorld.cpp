#include "StudentWorld.h"
#include <string>

#include "Actor.h"
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
	player->doSomething();

	//check if each actor is alive
	
	for (auto a : actors) {
		if (a->isAlive()) {
			//actors perform their action then check if player died or finished level
			a->doSomething();
			if (playerDiedDuringThisTick()) {
				return GWSTATUS_PLAYER_DIED;
			}
			if (playerCompletedCurrentLevel()) {
				playSound(GWSTATUS_FINISHED_LEVEL);
				return GWSTATUS_FINISHED_LEVEL;
			}
		}
	}
	//remove any actors that died
	removeDeadActors();


	if (playerDiedDuringThisTick()) {
		return GWSTATUS_PLAYER_DIED;
	}
	else {

		/*if (playerCompletedCurrentLevel()) {

			playSound(GWSTATUS_FINISHED_LEVEL);
			return GWSTATUS_FINISHED_LEVEL;
		}*/
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


//TODO: finish this
void StudentWorld::addNewActors() {
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
	int level = getLevel();
	int lives = getLives();
	// int health = getHP();
	// int water = getWaterAmount();
	// int gold = getGold();
	int barrelsLeft = remainingOil;
	//int sonar = getSonarAmount();
	int score = getScore();

	//need to make all of this into a string then format
	string s = "";
	//prints to the stats in game
	setGameStatText(s);

	return;
}

//issue: IceMan occasionally skips over the closest edge at the time of digging
void StudentWorld::playerDig(GraphObject::Direction dir, int x, int y) {
	switch (dir) {
	case GraphObject::right:
		if (x + 4 >= 64 || y + 3 >= 60)
			return;

		for (int i = 0; i < 4; i++) {
			if (icefield[x + 4][y + i] != nullptr) {
				playSound(SOUND_DIG);
				digIce(x + 3, y + i);
			}
		}
		break;


	case(GraphObject::left):
		if (x - 1 < 0 || y + 3 >= 60)
			return;
		for (int i = 0; i < 4; i++) {
			if (icefield[x - 1][y + i] != nullptr) {
				playSound(SOUND_DIG);
				digIce(x, y + i);
			}
		}
		break;

	case(GraphObject::down):
		if (y - 1 < 0)
			return;
		for (int i = 0; i < 4; i++) {
			if (icefield[x + i ][y - 1] != nullptr) {
				playSound(SOUND_DIG);
				digIce(x + i, y);
			}
		}
		break;

	case(GraphObject::up):
		if (y + 4 >= 60)
			return;
		for (int i = 0; i < 4; i++) {
			if (icefield[x + i][y + 1] != nullptr) {
				playSound(SOUND_DIG);
				digIce(x + i, y);
			}
		}
		break;
	}
}


void StudentWorld::digIce(int x, int y) {
	if (x < 0 || x > 64 || y < 0 || y > 60) { return; }
	if (icefield[x][y] == nullptr) { return; }

	icefield[x][y]->setVisible(false);
	delete icefield[x][y];
	icefield[x][y] = nullptr;
}

void StudentWorld::createOilField() {

	player = new IceMan(this, 30, 60);

	// test creating new Ice blocks
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 60; j++) {
			if (i >= 30 && i <= 33 && j > 3) { icefield[i][j] = nullptr; }
			else {
				icefield[i][j] = new Ice(this, i, j);
			}
		}
	}
}

bool StudentWorld::playerCompletedCurrentLevel() {
	if (remainingOil <= 0) {
		return true;
	}
	else {
		return false;
	}
}

bool StudentWorld::playerDiedDuringThisTick() {
	/*if (player->isAlive() == false) {
		return false;
	}
	else {
		decLives();
		return true;
	}
	*/
	return false;
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