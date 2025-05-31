#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

//TODO: for now all of these are dummy functions so that StudentWorld "works"



class Actor : public GraphObject {
private:
    bool alive;
    StudentWorld* world;

public:
    Actor(StudentWorld* sw, int startX, int startY, Direction startDir, int imageID, double size, int depth, bool visible);

    virtual ~Actor() {}

    StudentWorld* getWorld() const;

    virtual void doSomething() = 0;

    bool isAlive() const;

    void setAlive(bool state);
};




//All actor classes

class IceMan : public Actor {
public:
    IceMan(StudentWorld* sw, int startX, int startY);

    void doSomething();

};



class Ice : public Actor {
public:
    Ice(StudentWorld* sw, int startX, int startY)
        :Actor(sw, startX, startY, right, IID_ICE, 0.25, 3, true)
    {
        setVisible(true);
    }

    void doSomething();

};


/*
class Boulders : public Actor{
move();
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

*/

#endif // ACTOR_H_