#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//dummy version of actor TODO
Actor:: Actor(): GraphObject(){
        
};
//dummy version of Ice
class Ice : public Actor
{

};
bool Actor::isAlive() const
{
	return alive;
}
