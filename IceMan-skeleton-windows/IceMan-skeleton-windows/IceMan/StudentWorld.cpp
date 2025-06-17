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
	updateDisplayText();

	// creating either a water pool or a sonar kit
	int P = currentLevelNum * 30 + 290;
	
	if (rand() % P == 0) {
		if (rand() % 5 == 0) {
			if (canAddSonar()) {
				SonarKit* sonar = new SonarKit(this, 0, 60);
				actors.push_back(sonar);
			}
		}
		else {
			int randomX, randomY;
			do {
				randomX = rand() % 64;
				randomY = rand() % 60;
			} while (isOverlap(randomX, randomY) || isIce(randomX, randomY));
			WaterPool* water = new WaterPool(this, randomX, randomY);
			actors.push_back(water);
		}
	}
	
	// spawning either Regular or Hardcore Protesters
	int probOfHardcore = min(90, currentLevelNum * 10 + 30);
	/*
	if (canAddProtester())
	{
		if (rand() % 100 <= probOfHardcore)
			actors.push_back(new HardcoreProtesters(this)); 
		else
			actors.push_back(new RegularProtesters(this));
	}
	*/
	if (canAddProtester()) { 
		actors.push_back(new RegularProtester(this)); 
	}

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
		if (playerCompletedCurrentLevel()) {
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

void StudentWorld::removeDeadActors() {
	actors.erase(std::remove_if(actors.begin(), actors.end(),
		[](Actor* actor) {
			if (!actor->isAlive()) {
				actor->setVisible(false);
				delete actor;
				return true;
			}
			return false;
		}), actors.end());
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
	string s = formatDisplayText(level, lives, health, squirts, gold, barrelsLeft, sonar, score);
	//prints to the stats in game
	setGameStatText(s);

	return;
}

std::string StudentWorld::formatDisplayText(int level, int lives, int health, int squirts, int gold, int barrelsLeft, int sonar, int score)
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

void StudentWorld::playerDig(int x, int y) {
	for (int i = x; i < x + 4 && i < 64; ++i) {
		for (int j = y; j < y + 4 && j < 60; ++j) {
			if (icefield[i][j] != nullptr) {
				digIce(i, j);
				playSound(SOUND_DIG);
			}
		}
	}
}


void StudentWorld::digIce(int x, int y) {
	if (x < 0 || x > 64 || y < 0 || y > 60) { return; }
	if (icefield[x][y] == nullptr) { return; }

	icefield[x][y]->setVisible(false);
	delete icefield[x][y];
	icefield[x][y] = nullptr;
}

bool StudentWorld::canMove(int x, int y) {
	return (x >= 0 && x < 64 && y >= 0 && y < 64 && !isBoulder(x,y));
}

bool StudentWorld::isNearIceMan(Actor* obj, float radius) {
	float objX = obj->getX();
	float objY = obj->getY();

	float distance = sqrt(pow(objX - player->getX(), 2) + pow(objY - player->getY(), 2));
	return distance <= radius;
}

void StudentWorld::createOilField() {
	currentLevelNum = getLevel();
	remainingOilBarrels = 0;
	player = new IceMan(this, 30, 60);

	// creating ice blocks
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 60; j++) {
			if (i >= 30 && i <= 33 && j > 3) { icefield[i][j] = nullptr; }
			else {
				icefield[i][j] = new Ice(this, i, j);
			}
		}
	}

	// creating oil barrels	
	int L = min(2 + currentLevelNum, 21);
	for (int i = 0; i < L; i++) {
		int randomX, randomY;
		do {
			randomX = rand() % 64;
			randomY = rand() % 59;
		} while (isOverlap(randomX, randomY) && randomX >= 30 && randomX <= 33 && randomY > 3);
		OilBarrel* barrel = new OilBarrel(this, randomX, randomY);
		actors.push_back(barrel);
		remainingOilBarrels++;
	}

	// creating gold nuggets
	int G = max(5 - currentLevelNum / 2, 2);
	for (int i = 0; i < G; i++) {
		int randomX, randomY;
		do {
			randomX = rand() % 64;
			randomY = rand() % 59;
		} while (isOverlap(randomX, randomY) && randomX >= 30 && randomX <= 33 && randomY > 3);
		GoldNugget* gold = new GoldNugget(this, randomX, randomY, false);
		actors.push_back(gold);
	}

	//creating protesters
	ticksToWaitToAddProtester = ticksSinceLastProtester = max(25, 200 - currentLevelNum);
}

void StudentWorld::barrelsToPickUp() {
	remainingOilBarrels--;
}

bool StudentWorld::playerCompletedCurrentLevel() {
	if (remainingOilBarrels == 0) {
		playSound(SOUND_FINISHED_LEVEL);
		return true;
	}
	else {
		return false;
	}
}

bool StudentWorld::playerDiedDuringThisTick() {
	if (player->isAlive() == false) {
		return false;
	}
	else {
		//decLives();
		return false;
	}
	
	return false;
}

bool StudentWorld::isOverlap(int x, int y) {
	for (auto a : actors) {
		if (a->isAlive()) {
			if (distance(a->getX(), x, a->getY(), y) <= 6) {
				return true;
			}
		}
	}
	return false;
}

double StudentWorld::distance(int x1, int y1, int x2, int y2) const
{
	return sqrt((pow(x2 - x1, 2) + pow(y2 - y1, 2)));
}

void StudentWorld::dropGold(int x, int y) {
	GoldNugget* gold = new GoldNugget(this, x, y, true);
	gold->setPickupAble(false);
	gold->setTicksToLive(100);
	actors.push_back(gold);
}

bool StudentWorld::canAddSonar() {
	for (auto a : actors) {
		if (dynamic_cast<SonarKit*>(a) && a->isAlive()) {
			return false;
		}
	}
	return true;
}

bool StudentWorld::isIce(int x, int y) {
	if (x < 0 || x + 3 >= 64 || y < 0 || y + 3 >= 60) return true; 

	// Check the 4x4 area
	for (int i = x; i < x + 4; ++i) {
		for (int j = y; j < y + 4; ++j) {
			if (icefield[i][j] != nullptr) {
				return true; 
			}
		}
	}

	return false; 
}

void StudentWorld::illuminate(int x, int y) {
	playSound(SOUND_SONAR);
	for (auto a : actors) {
		if (isNearIceMan(a, 12)) {
			a->setVisible(true);
		}
	}
}

bool StudentWorld::isBoulder(int x, int y) {
	for (auto a : actors) {
		for (int i = x; i < x + 4; ++i) {
			for (int j = y; j < y + 4; ++j) {
				if (dynamic_cast<Boulder*>(a) && a->isAlive()) {
					return true;
				}
			}
		}
	}
	return false;
}

bool StudentWorld::clearVerticalPath(Protester* p) {
	if (player->getX() == p->getX()) {
		int start = min(player->getY(), p->getY()) + 1;
		int end = max(player->getY(), p->getY()) + 1;

		for (int i = start; i < end; i++) {
			if (isIce(p->getX(), i) || isBoulder(p->getX(), i)) {
				return false;
			}
		}
		return true;
	}
	return false;
}

bool StudentWorld::canAddProtester() {
	if (ticksSinceLastProtester >= ticksToWaitToAddProtester && numProtesters < min(15, static_cast<int>(2 + currentLevelNum * 1.5)))
	{
		ticksSinceLastProtester = 0;
		numProtesters++;
		return true;
	}


	ticksSinceLastProtester++;
	return false;
}

bool StudentWorld::clearHorizontalPath(Protester* p) {
	// horizontal path
	if (player->getY() == p->getY()) {
		int start = min(player->getX(), p->getX()) + 1;
		int end = max(player->getX(), p->getX()) + 1;

		for (int i = start; i < end; i++) {
			if (isIce(i, p->getY()) || isBoulder(i, p->getY())) {
				return false;
			}
		}
		return true;
	}
	return false;
}

void StudentWorld::addActor(Actor* a)
{
	actors.push_back(a);
}

bool StudentWorld::scanIce(Actor* a, GraphObject::Direction dir, int numTiles)
{
	int xCord = a->getX();
	int yCord = a->getY();
	int tile = 1;
	switch (dir) {
	case(GraphObject::right):
		while (tile <= numTiles) {
			if (xCord + tile >= VIEW_WIDTH) {
				return false;
			}
			for (int i = 0; i < SPRITE_HEIGHT; i++) {
				if (icefield[xCord + SPRITE_WIDTH + tile][yCord + i] != nullptr)
					return false;
			}
			tile++;
		}
		return true;
		break;
	case(GraphObject::left):
		while (tile <= numTiles) {
			if (xCord - tile < 0) {
				return false;
			}
			for (int i = 0; i < SPRITE_HEIGHT; i++) {
				if (icefield[xCord - tile][yCord + i] != nullptr)
					return false;
			}
			tile++;
		}
		break;
	case(GraphObject::up):
		while (tile <= numTiles) {
			if (yCord + tile >= VIEW_HEIGHT) {
				return false;
			}
			for (int i = 0; i < SPRITE_HEIGHT; i++) {
				if (icefield[xCord + i][yCord + SPRITE_HEIGHT + tile] != nullptr)
					return false;
			}
			tile++;
		}
		break;
	case(GraphObject::down):
		while (tile <= numTiles) {
			if (yCord - tile < 0) {
				return false;
			}
			for (int i = 0; i < SPRITE_HEIGHT; i++) {
				if (icefield[xCord + i][yCord - tile] != nullptr)
					return false;
			}
			tile++;
		}
		break;
	}
	return true;
}

int StudentWorld::getCurrentGameLevel() const
{
	return getLevel();
}

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
	return remainingOilBarrels;
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