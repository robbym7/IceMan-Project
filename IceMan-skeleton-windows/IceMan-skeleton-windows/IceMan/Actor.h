#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

//TODO: for now all of these are dummy functions so that StudentWorld "works"



class Actor : public GraphObject {
public:
    Actor(StudentWorld* world, int startX, int startY, Direction startDir,
        bool visible, int imageID, double size, int depth);
   
    virtual void move() = 0;

    bool isAlive() const;

    
private:
    bool alive;
};

 






//All actor classes

class IceMan : public Actor {
    void IceMan:: move();
};

class Ice : public Actor{
void Ice:: move();
};

class Boulders : public Actor{
void Boulders:: move();
};

class Squirt : public Actor{
void Squirt:: move();
};

class OilBarrel : public Actor{
void OilBarrel:: move();
};

class GoldNugget : public Actor{
void GoldNugget:: move();
};

class SonarKit : public Actor{
void SonarKit:: move();
};

class WaterPool : public Actor{
void WaterPool:: move();
};

class Protester: public Actor {
public:

};

class HardcoreProtester: public Protester {
public:
};


#endif // ACTOR_H_
