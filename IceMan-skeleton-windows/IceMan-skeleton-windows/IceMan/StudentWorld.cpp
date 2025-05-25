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
	createOilField();

	return GWSTATUS_CONTINUE_GAME;
}
else{

	return GWSTATUS_PLAYER_DIED;
}
	
}
int StudentWorld:: move(){
	updateDisplayText();

	//player needs to move first 

	//check if each actor is alive
	for (auto a : actors){
		if(a->isAlive()){
			//actors perform their action then check if player died or finished level
			a->move();
			if(playerDiedDuringThisTick()){
				return GWSTATUS_PLAYER_DIED;
			}
			if(playerCompletedCurrentLevel()){
				playFinishedLevelSound();
				return GWSTATUS_FINISHED_LEVEL;
			}
		}
	}
	//remove any actors that died
	removeDeadActors();

	
	if(playerDiedDuringThisTick()){
		return GWSTATUS_PLAYER_DIED;
	}
	else{

		if(playerCompletedCurrentLevel()){

			playFinishedLevelSound();
			return GWSTATUS_FINISHED_LEVEL;
		}
		//if player hasnt died or finished the level then continue the game
		return GWSTATUS_CONTINUE_GAME;
	}
}

//is called when player loses a life
void StudentWorld:: cleanUp(){
	
	for(auto a : actors){
		delete a;
	}
	//actors.clear memory leaks if you dont delete one by one first
	actors.clear();

	//need to delete player since stored seperate from actors
	if(player != nullptr){
		delete player;
		player =  nullptr;
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

//should have a 2D array of ice actors
//should place the IceMan 
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