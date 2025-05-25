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
	StudentWorld(std::string assetDir) : GameWorld(assetDir){}
	virtual ~StudentWorld();

	virtual int init();
    virtual int move();
    virtual void cleanUp();
	
	//Add New Actors During Each Tick
	void addNewActors();

	//Remove Dead Actors after Each Tick
	void removeDeadActors();

	//Updating the Display Text
	void updateDisplayText();

	
private:
	//creates the oilField
	void createOilField();

	bool playerCompletedCurrentLevel();

	bool playerDiedDuringThisTick();

	

	vector<Actor *> actors;
	Ice* icefield[VIEW_WIDTH][VIEW_HEIGHT];
	IceMan* player;


	int remainingOil;
	//everything that changes from level to level
	int currentLevel;
	int numBoulders;
	int numGoldNuggets;
	int numOilBarrels;




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
