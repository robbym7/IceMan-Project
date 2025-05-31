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
	player->move();
	//check if each actor is alive
	for (auto a : actors) {
		if (a->isAlive()) {
			//actors perform their action then check if player died or finished level
			a->move();
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
	for (int i = 0; i < VIEW_WIDTH; i++) {
		for (int j = 0; j < VIEW_HEIGHT; j++) {
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

//should have a 2D array of ice actors
//should place the IceMan 
void StudentWorld::createOilField() {

	player = new IceMan(this, 30, 60);

	// test creating new Ice blocks
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 60; j++) {
			icefield[i][j] = new Ice(this, i, j);
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


void StudentWorld::playerDig(GraphObject::Direction dir, int x, int y)
{
	switch (dir) {
	case(GraphObject::right):
		if (x > VIEW_WIDTH - SPRITE_WIDTH) {
			return;
		}
		for (int i = 0; i < SPRITE_WIDTH; i++) {
			if (y + i < VIEW_HEIGHT) {
				if (icefield[x + 3][y + i] != nullptr) {
					playSound(SOUND_DIG);
					delete icefield[x + 3][y + i];
					icefield[x + 3][y + i] = nullptr;
				}
			}
		}
		break;

		//only ever does this first one anyway
	//case(GraphObject::left):
	//	if (x < 0)
	//		return;
	//	for (int i = 0; i < SPRITE_WIDTH; i++) {
	//		if (y + i < VIEW_HEIGHT) {
	//			if (icefield[x][y + i] != nullptr) {
	//				playSound(SOUND_DIG);
	//				delete icefield[x][y + i];
	//				icefield[x][y + i] = nullptr;
	//			}
	//		}
	//	}
	//	break;
	//case(GraphObject::down):
	//	if (y < 0)
	//		return;
	//	for (int i = 0; i < SPRITE_HEIGHT; i++) {
	//		if (x + i < VIEW_WIDTH) {
	//			if (icefield[x + i][y] != nullptr) {
	//				playSound(SOUND_DIG);
	//				delete icefield[x + i][y];
	//				icefield[x + i][y] = nullptr;
	//			}
	//		}
	//	}
	//	break;
	//case(GraphObject::up):
	//	if (y > VIEW_HEIGHT - SPRITE_HEIGHT)
	//		return;
	//	for (int i = 0; i < SPRITE_HEIGHT; i++) {
	//		if (y + 3 < VIEW_HEIGHT) {
	//			if (icefield[x + i][y + 3] != nullptr) {
	//				playSound(SOUND_DIG);
	//				delete icefield[x + i][y + 3];
	//				icefield[x + i][y + 3] = nullptr;
	//			}
	//		}
	//	}
	//	break;
	//}
	//return;
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