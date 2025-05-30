#include "StudentWorld.h"
#include <string>

#include "Actor.h"
#include <vector>
using namespace std;


GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}
// StudentWorld::StudentWorld(std::string assetDir): GameWorld(assetDir){
// }

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
				playSound(GWSTATUS_FINISHED_LEVEL);
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

			playSound(GWSTATUS_FINISHED_LEVEL);
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
//destructor
StudentWorld::~StudentWorld() { cleanUp(); }


//TODO: finish this
void StudentWorld::addNewActors(){
return;
}

void StudentWorld::removeDeadActors(){

	for (auto a : actors){
		if(a->isAlive() == false){
			a->setVisible(false);
		}
	}
	return;
}

//TODO: commented ones rely on items being done(names can change)
//example:     Lvl: 52 Lives: 3 Hlth: 80% Wtr: 20 Gld: 3 Oil Left: 2 Sonar: 1 Scr: 321000
void StudentWorld::updateDisplayText(){
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
void StudentWorld::createOilField(){
return;
}

bool StudentWorld::playerCompletedCurrentLevel(){
	if (remainingOil <= 0){
		return true;
	}
	else {
		return false;
	}
}

bool StudentWorld::playerDiedDuringThisTick(){
	if(player->isAlive() == false){
		return false;
	}
	else{
		decLives();
		return true;
	}
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