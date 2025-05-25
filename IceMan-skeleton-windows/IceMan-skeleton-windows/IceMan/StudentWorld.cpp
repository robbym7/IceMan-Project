#include "StudentWorld.h"
#include <string>

#include "Actor.h"
#include <vector>
using namespace std;


GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}
StudentWorld::StudentWorld(std::string assetDir): GameWorld(assetDir){
}

int StudentWorld:: init(){
	if (getLives() > 0){

	
	vector<Actor *> actors;

	//should have a 2D array of ice actors
	//should place the IceMan 
	createOilField();

	return GWSTATUS_CONTINUE_GAME;
}
else{
	return GWSTATUS_PLAYER_DIED;
}
	
}
int StudentWorld:: move(){
	updateDisplayText();
	actorsDoSomething();
	removeDeadActors();
	if(isAlive() = true){

	}

}
void StudentWorld:: cleanUp(){
	
}

void createOilField(){

}
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