#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <vector>
class StudentWorld;

class Actor : public GraphObject {
private:
    bool alive;
    StudentWorld* world;

public:
    Actor(StudentWorld* sw, int startX, int startY, Direction startDir, int imageID, double size, int depth, bool visible);

    virtual ~Actor() {}

    StudentWorld* getWorld() const;

    bool moveToIfPossible(int x, int y);

    virtual void move() = 0;

    bool isAlive() const;

    void setDead();

    // Annoy this actor.
    //virtual bool annoy(unsigned int amt);

    int getTickCounter() const;

    void increaseTick();

    unsigned int tickCounter;
};

class Agent : public Actor
{
public:
    Agent(StudentWorld* world, int startX, int startY, Direction startDir,
        int imageID, unsigned int hitPoints);

    // Pick up a gold nugget.
    virtual void addGold() = 0;

    // How many hit points does this actor have left?
    unsigned int getHitPoints() const;

    //virtual bool annoy(unsigned int amount);
    //virtual bool canPickThingsUp() const;
private:
    unsigned int hp = 10;
};

class IceMan : public Agent {
public:
    IceMan(StudentWorld* sw, int startX, int startY);
    virtual void move();
    virtual void addGold();
    virtual void addSonar();
    virtual void addWater();

    unsigned int getGold() const;

    // Get amount of sonar charges
    unsigned int getSonar() const;

    // Get amount of water
    unsigned int getWater() const;

private:
    int m_gold;
    int m_water;
    int m_sonar;
    unsigned int waterCharge;
};

class Protester : public Agent {
public:
    Protester(StudentWorld* sw, int imageID, int hitPoints);
    
    virtual void move() = 0;
    virtual void addGold();
    //virtual bool annoy(int amount);
    //virtual bool huntsIceMan() const;

    // Set state to having given up protest
    void setMustLeaveOilField() { m_leaveOilField = true; }
    bool isLeavingOilField() { return m_leaveOilField; }

    void setRestingTicks(int ticks) { m_ticksToWaitBetweenMoves = ticks; }
    int getRestingTicks() const { return m_ticksToWaitBetweenMoves; }

    void resetNonRestingTicks();
    int getNonRestingTicks() const { return m_nonRestingTicks; }
    
    void setNumSquaresToMove(int num) { m_numSquaresToMoveInCurrentDirection = num; }
private:
    int m_numSquaresToMoveInCurrentDirection;
    int m_ticksToWaitBetweenMoves;
    int m_nonRestingTicks;
    bool m_leaveOilField;
};

class RegularProtester : public Protester {
public:
    RegularProtester(StudentWorld* world)
        : Protester(world, IID_PROTESTER, 5) { }
    virtual void addGold() {}
    virtual void move();
};

class Ice : public Actor {
public:
    Ice(StudentWorld* sw, int startX, int startY);

    ~Ice() {}

    void move();

};

class Boulder : public Actor
{
public:
    Boulder(StudentWorld* world, int startX, int startY);
    virtual void move();
    virtual bool canActorsPassThroughMe() const;
private:
    bool falling;
};

class Goodies : public Actor {
public:
    Goodies(StudentWorld* world, int startX, int startY, int imageID, int soundToPlay, bool activateOnPlayer, bool activateOnProtestor, bool initiallyActive);  
    virtual void move();
    void setTicksToLive(int n);    
    int getTicksToLive() { return ticksToLive; }
    bool isPickupAble() { return m_activateOnPlayer; }
    void setPickupAble(bool canPickup) { m_activateOnPlayer = canPickup; }
    void decreaseTick();

private:
    int ticksToLive;
    bool m_activateOnPlayer;
};

class OilBarrel : public Goodies {
public:
    OilBarrel(StudentWorld* world, int startX, int startY);
    virtual void move();
};

class GoldNugget : public Goodies {
public:
    GoldNugget(StudentWorld* world, int startX, int startY, bool temp);
    virtual void move();

    bool isTemporary() const;

private:
    bool m_tempState;
};

class SonarKit : public Goodies {
public:
    SonarKit(StudentWorld* world, int startX, int startY);
    virtual void move();
};

class WaterPool : public Goodies {
public:
    WaterPool(StudentWorld* world, int startX, int startY);
    virtual void move();
};

class Squirt : public Actor {
public:
    Squirt(StudentWorld* world, int startX, int startY, Direction startDir);
    virtual void move();
private:
    int distanceTraveled;
};

#endif // ACTOR_H_