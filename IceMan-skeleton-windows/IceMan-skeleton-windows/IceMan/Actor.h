#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <vector>
class StudentWorld;

class Actor : public GraphObject {
public:
    Actor(StudentWorld* sw, int startX, int startY, Direction startDir, int imageID, double size, int depth, bool visible);

    StudentWorld* getWorld() const;

    bool moveToIfPossible(int x, int y);

    // Action to perform each tick.
    virtual void move() = 0;

    // Is this actor alive?
    bool isAlive() const;

    // Mark this actor as dead.
    void setDead();

    // Annoy this actor.
    virtual bool annoy(unsigned int amt);

    int getTickCounter() const;

    void increaseTick();

    unsigned int tickCounter;

private:
    bool alive;


    StudentWorld* world;

};

class Squirt : public Actor
{
public:
    Squirt(StudentWorld* world, int startX, int startY, Direction startDir);
    virtual void move();
private:
    int distanceTraveled;
};






//All actor classes

// class IceMan : public Actor {
// public:
//     IceMan(StudentWorld* sw, int startX, int startY);
//     void move();

// };



//
class Agent : public Actor
{
public:
    Agent(StudentWorld* world, int startX, int startY, Direction startDir,
        int imageID, unsigned int hitPoints);

    // Pick up a gold nugget.
    virtual void addGold() = 0;

    // How many hit points does this actor have left?
    unsigned int getHitPoints() const;

    virtual bool annoy(unsigned int amount);
    virtual bool canPickThingsUp() const;
private:
    unsigned int hp;
};

class IceMan : public Agent
{
public:
    IceMan(StudentWorld* world, int startX, int startY);
    virtual void move();
    virtual bool annoy(unsigned int amount);
    virtual void addGold();
    virtual bool canDigThroughIce() const;

    // Pick up a sonar kit.
    void addSonar();

    // Pick up water.
    void addWater();

    // Get amount of gold
    unsigned int getGold() const;

    // Get amount of sonar charges
    unsigned int getSonar() const;

    // Get amount of water
    unsigned int getWater() const;
};

class Protester : public Agent
{
public:
    Protester(StudentWorld* world, int startX, int startY, int imageID,
        unsigned int hitPoints, unsigned int score);
    virtual void move();
    virtual bool annoy(unsigned int amount);
    virtual void addGold();
    virtual bool huntsIceMan() const;
    bool canPickDroppedGoldUp() const;

    // Set state to having given up protest
    void setMustLeaveOilField();

    // Set number of ticks until next move
    void setTicksToNextMove();

    int getTicksToNextMove() const;

    bool getWantsToLeaveOilField() const;

    void incNonRestingTick();

    int getNonRestingTick() const;

    void resetNonRestingTick();




    bool wantsToLeaveOilField;
    int ticksBetweenMoves;
    int nonRestingTick;
};

class RegularProtester : public Protester
{
public:
    RegularProtester(StudentWorld* world, int startX, int startY, int imageID);
    virtual void move();
    virtual void addGold();

};

class HardcoreProtester : public Protester
{
public:
    HardcoreProtester(StudentWorld* world, int startX, int startY, int imageID);
    virtual void move();
    virtual void addGold();
};

class Ice : public Actor
{
public:
    Ice(StudentWorld* world, int stsartX, int startY);
    virtual void move();
};




std::vector<std::vector<int>> pathFind(Ice* icefield[VIEW_WIDTH][VIEW_HEIGHT]);

#endif // ACTOR_H_