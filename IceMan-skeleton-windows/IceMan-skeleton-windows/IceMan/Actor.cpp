#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//dummy version of actor TODO
Actor:: Actor(StudentWorld* world, int startX, int startY, Direction startDir,
        bool visible, int imageID, double size, int depth): GraphObject(imageID, startX, startY, startDir, size, depth){
        
};
//dummy version of Ice
class Ice : public Actor
{

};
bool Actor::isAlive() const
{
	return alive;
}
