#include "Actor.h"
#include "StudentWorld.h"

//TODO: for now all of these are dummy functions so that StudentWorld "works"


Actor:: Actor(StudentWorld* world, int startX, int startY, Direction startDir,
        bool visible, int imageID, double size, int depth): GraphObject(imageID, startX, startY, startDir, size, depth){
        
};


void IceMan:: move(){};

void Ice:: move(){};

void Boulders:: move(){};

void Squirt:: move(){};

void OilBarrel:: move(){};

void GoldNugget:: move(){};

void SonarKit:: move(){};

void WaterPool:: move(){};


bool Actor::isAlive() const
{
	return alive;
}
