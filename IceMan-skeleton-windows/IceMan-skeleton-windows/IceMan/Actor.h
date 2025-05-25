#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

//TODO: for now all of these are dummy functions so that StudentWorld "works"

class Actor : public GraphObject {
public:
    Actor(StudentWorld* world, int startX, int startY, Direction startDir,
        bool visible, int imageID, double size, int depth);
    //gets player_alive
    bool isAlive() const;


private:
    bool alive = true;
};

class IceMan : public Actor {};

class Ice : public Actor{};

class Boulders : public Actor{};

class Squirt : public Actor{};

class OilBarrel : public Actor{};

class GoldNugget : public Actor{};

class SonarKit : public Actor{};

class WaterPool : public Actor{};

class Protester: public Actor {
public:
};

class HardcoreProtester: public Protester {
public:
};


#endif // ACTOR_H_
