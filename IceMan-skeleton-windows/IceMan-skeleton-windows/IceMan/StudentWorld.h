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

	void playerDig(GraphObject::Direction dir, int x, int y);

	void digIce(int x, int y);

	bool canMove(int x, int y);

	bool isNearIceMan(Actor* a, int radius) const;
	
    bool isBoulder(int x, int y);
    
    bool scanIce(Actor* a, GraphObject::Direction dir, int numTiles);

    bool scanPlayer(Actor* a, int radius);

	int getRemainingOil() const;
	

	IceMan* getPlayer();

	//creates the oilField
	void createOilField();
	void generateBoulders(int totalBoulders);
	bool doesOverlap(int x, int y, double radius);
	
	bool playerCompletedCurrentLevel() const;

	bool playerDiedDuringThisTick();

	Ice* icefield[VIEW_WIDTH][VIEW_HEIGHT];
private:
	

	std::vector <Actor*> actors;

	IceMan* player;
	
	double distance(int x1, int y1, int x2, int y2) const;
	int remainingOil;
	//everything that changes based on the level number
	int currentLevel;
	int numBoulders;
	int numGold;
	int numOilBarrels;

	//display text getters
	std::string formatDisplayTest(int level, int lives, int health,int squirts, int gold, int barrelsLeft, int sonar, int score);
    int getCurrentGameLevel() const;
    int getNumLivesLeft() const;
    int getCurrentHealth() const;
    int getSquirtsLeftInSquirtGun() const;
    int getPlayerGoldCount() const;
    int getNumberOfBarrelsRemainingToBePickedUp() const;
    int getPlayerSonarChargeCount() const;
    int getCurrentScore() const;
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
