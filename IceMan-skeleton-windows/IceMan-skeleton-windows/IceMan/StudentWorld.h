#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>

#include "Actor.h"
#include <vector>


class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir) : GameWorld(assetDir) {}
	virtual ~StudentWorld();

	virtual int init();
	virtual int move();
	virtual void cleanUp();

	//Add New Actors During Each Tick
	void addActor(Actor* a);

	//Remove Dead Actors after Each Tick
	void removeDeadActors();

	//Updating the Display Text
	void updateDisplayText();

	void playerDig(int x, int y);

	void digIce(int x, int y);

	bool canMove(int x, int y);

	bool isNearIceMan(Actor* obj, float radius);

	void barrelsToPickUp();

	//creates the oilField
	void createOilField();

	bool playerCompletedCurrentLevel();

	bool playerDiedDuringThisTick();

	std::vector <Actor*> actors;
	Ice* icefield[VIEW_WIDTH][60];
	IceMan* player;

	double distance(int x1, int y1, int x2, int y2) const;

	bool isOverlap(int x, int y);

	void dropGold(int x, int y);

	bool canAddSonar();

	bool isIce(int x, int y);

	void illuminate(int x, int y);

	bool isBoulder(int x, int y);

	bool scanIce(Actor* a, GraphObject::Direction dir, int numTiles);

	bool scanPlayer(Actor* a, int radius);

	bool canAddProtester();

	bool StudentWorld::clearVerticalPath(Protester* p);
	bool StudentWorld::clearHorizontalPath(Protester* p);
	//everything that changes based on the level number
	int currentLevelNum;
	int numBoulders;
	int numGoldNuggets;
	int remainingOilBarrels;
	int ticksSinceLastProtester;
	int ticksToWaitToAddProtester;
	int numProtesters;

	//display text getters
	std::string formatDisplayText(int level, int lives, int health, int squirts, int gold, int barrelsLeft, int sonar, int score);
	int getCurrentGameLevel() const;
	int getNumLivesLeft() const;
	int getCurrentHealth() const;
	int getSquirtsLeftInSquirtGun() const;
	int getPlayerGoldCount() const;
	int getNumberOfBarrelsRemainingToBePickedUp() const;
	int getPlayerSonarChargeCount() const;
	int getCurrentScore() const;
	IceMan* getPlayer();
};

/*

//when the game first starts, when the player completes the current, when the player loses a life (but has more lives left
virtual int init(){
	return GWSTATUS_CONTINUE_GAME;
}

virtual int move(){
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	decLives();
	return GWSTATUS_PLAYER_DIED;
}

// virtual void cleanUp(){
// }

*/

#endif // STUDENTWORLD_H_
