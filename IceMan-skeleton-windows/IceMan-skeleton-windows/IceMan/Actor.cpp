#include "Actor.h"
#include "StudentWorld.h"

#include <vector>
#include <queue>

using namespace std; 

//constructor for actor
Actor::Actor(StudentWorld* sw, int startX, int startY, Direction startDir, int imageID, double size, int depth, bool visible)
    :GraphObject(imageID, startX, startY, startDir, size, depth)
    ,alive(true), world(sw)
{
    setVisible(visible);
}

bool Actor::isAlive() const
{
    return alive;
}

StudentWorld* Actor::getWorld() const
{
    return world;
}

void Actor::setDead() { alive = false; }

int Actor::getTickCounter()const
{
    return tickCounter;
}

void Actor::increaseTick()
{
    tickCounter++;
}

bool Actor::moveToIfPossible(int x, int y)
{
    if ((x >= 0 && x <= VIEW_WIDTH - SPRITE_WIDTH) && (y >= 0 && y <= VIEW_HEIGHT - SPRITE_HEIGHT)) {
        moveTo(x, y);
        return true;
    }
    return false;
}

Agent::Agent(StudentWorld* world, int startX, int startY, Direction startDir, int imageID, unsigned int hitPoints) :
    Actor(world, startX, startY, startDir, imageID, 1.0, 0, true)
    , hp(hitPoints) { }

unsigned int Agent::getHitPoints() const { return hp; }

IceMan::IceMan(StudentWorld* sw, int startX, int startY)
    :Agent(sw, startX, startY, right, IID_PLAYER, 10)
{
    setVisible(true);
    m_gold = 0;
    m_water = 5;
    m_sonar = 1;
}

void IceMan::move() {

    int input;
    StudentWorld* sw = getWorld();
    Direction dir = getDirection();
    if (sw->getKey(input) == true) {
        switch (input) {
        case KEY_PRESS_LEFT:
            if (dir != left) {
                setDirection(left);
            }
            else {
                if (sw->canMove(getX() - 1, getY())) {
                    moveTo(getX() - 1, getY());
                    sw->playerDig(getX(), getY());
                }
            }
            break;
        case KEY_PRESS_RIGHT:
            if (dir != right) {
                setDirection(right);
            }
            else {
                if (sw->canMove(getX() + 1, getY())) {
                    moveTo(getX() + 1, getY());
                    sw->playerDig(getX(), getY());
                }
            }
            break;
        case KEY_PRESS_UP:
            if (dir != up) {
                setDirection(up);
            }
            else {
                if (sw->canMove(getX(), getY() + 1)) {
                    moveTo(getX(), getY() + 1);
                    sw->playerDig(getX(), getY());
                }
            }
            break;
        case KEY_PRESS_DOWN:
            if (dir != down) {
                setDirection(down);
            }
            else {
                if (sw->canMove(getX(), getY() - 1)) {
                    moveTo(getX(), getY() - 1);
                    sw->playerDig(getX(), getY());
                }
            }
            break;
        case KEY_PRESS_TAB:
            if (m_gold > 0) {
                sw->dropGold(getX(), getY());
                m_gold--;
            }
            break;

        case 'z':
        case 'Z':
            if (m_sonar > 0) {
                sw->illuminate(getX(), getY());
                m_sonar--;
            }
            break;

        case KEY_PRESS_SPACE:
            //Implement the code to use Squirt
            if (waterCharge <= 0)
                return;
            if (waterCharge > 0) {
                sw->addActor(new Squirt(sw, getX(), getY(), getDirection()));
                sw->playSound(SOUND_PLAYER_SQUIRT);
                waterCharge--;
            }

        }
    }
}

void IceMan::addGold() { m_gold++; }

void IceMan::addSonar() { m_sonar++; }

void IceMan::addWater() { m_water++; }

unsigned int IceMan::getWater() const {
    return m_water;
}
unsigned int IceMan::getSonar() const {
    return m_sonar;
}
unsigned int IceMan::getGold() const {
    return m_gold;
}

Ice::Ice(StudentWorld* sw, int startX, int startY)
    : Actor(sw, startX, startY, right, IID_ICE, 0.25, 3, true)
{
    setVisible(true);
}

Protester::Protester(StudentWorld* sw, int imageID, int hitPoints)
    : Agent(sw, 60, 60, left, imageID, hitPoints) 
{
    m_ticksToWaitBetweenMoves = max(0, 3 - sw->currentLevelNum);
    m_leaveOilField = false;
    m_nonRestingTicks = 0;
    m_numSquaresToMoveInCurrentDirection = 0;
}

void Protester::addGold() { }

void RegularProtester::move() {
    if (!isAlive()) return;

    if (getRestingTicks() > 0) {
        setRestingTicks(getRestingTicks() - 1);
        return;
    }

    IceMan* iceman = getWorld()->player;
    if (isLeavingOilField()) {

    }

    // check if iceman is within line of sight
    if (iceman->getX() == getX() || iceman->getY() == getY()) {
        if (getWorld()->distance(iceman->getX(), iceman->getY(), getX(), getY()) >= 4.0) {
            // check if there's a clear path
            if (getWorld()->clearVerticalPath(this)) {
                // iceman is below protester
                if (iceman->getY() < getY()) {
                    setDirection(down);
                    moveTo(getX(), getY() - 1);
                    setNumSquaresToMove(0);
                }
                // iceman is above protester    
                else {
                    setDirection(up);
                    moveTo(getX(), getY() + 1);
                    setNumSquaresToMove(0);
                }               
            }
 
            else if (getWorld()->clearHorizontalPath(this)) {
                // iceman is to the left of protester
                if (iceman->getX() < getX()) {
                    setDirection(left);
                    moveTo(getX() - 1, getY());
                    setNumSquaresToMove(0);
                }
                // iceman is to the right of protester    
                else {
                    setDirection(right);
                    moveTo(getX() + 1, getY());
                    setNumSquaresToMove(0);
                }
            }
        }
    }
}

void Ice::move() { ; };

void Boulder::move() {
    //Student World will deal with deleting the boulder;
    if (isAlive() == false)
        return;
    /*
    if (falling == false && getWorld()->scanIce(this, down, 1) == true) {
        falling = true;
    }
    if (falling && getTickCounter() < 30) {
        increaseTick();
    }
    if (falling && getTickCounter() >= 30) {
        //makes it so sound plays once
        if (getTickCounter() == 30) {
            getWorld()->playSound(SOUND_FALLING_ROCK);
            increaseTick();
        }
        if (getWorld()->isNearIceMan(this, 3)) {
            getWorld()->player->setDead();
        }
        if (getX() != 0 && getWorld()->scanIce(this, down, 1) == true) {
            moveToIfPossible(getX(), getY() - 1);
        }
        else {
            setDead();
        }
    }
    */
}

bool Boulder::canActorsPassThroughMe() const
{
    return false;
}

Goodies::Goodies(StudentWorld* world, int startX, int startY, int imageID, int soundToPlay, bool activateOnPlayer, bool activateOnProtestor, bool initiallyActive)
    :Actor(world, startX, startY, right, imageID, 1.0, 2, initiallyActive) 
{
    m_activateOnPlayer = activateOnPlayer;
}

void Goodies::move() { ; }

void Goodies::decreaseTick()
{
    ticksToLive--;
}

void Goodies::setTicksToLive(int n) { 
    ticksToLive = n; 
}

OilBarrel::OilBarrel(StudentWorld* world, int startX, int startY)
    :Goodies(world, startX, startY, IID_BARREL, SOUND_FOUND_OIL, true, false, false) {}

void OilBarrel::move() { 
    if (!isAlive()) { return; }

    if (!isVisible() && getWorld()->isNearIceMan(this, 4.0)) {
        setVisible(true);
        return;
    }

    if (getWorld()->isNearIceMan(this, 3.0)) {
        setDead();
        getWorld()->playSound(SOUND_FOUND_OIL);
        getWorld()->increaseScore(1000);
        getWorld()->barrelsToPickUp();
    }
}

GoldNugget::GoldNugget(StudentWorld* world, int startX, int startY, bool temp)
    :Goodies(world, startX, startY, IID_GOLD, SOUND_GOT_GOODIE, true, false, false) 
{
    m_tempState = temp;
}

void GoldNugget::move() {
    if (!isAlive()) { return; }

    if (!isVisible() && getWorld()->isNearIceMan(this, 4.0)) {
        setVisible(true);
        return;
    }

    if (isPickupAble() && getWorld()->isNearIceMan(this, 3.0)) {
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(10);
        getWorld()->player->addGold();
    }

    else if (!isPickupAble()) {}
}

bool GoldNugget::isTemporary() const { return m_tempState; }

SonarKit::SonarKit(StudentWorld* world, int startX, int startY) 
    :Goodies(world, startX, startY, IID_SONAR, SOUND_GOT_GOODIE, true, false, true) 
{ 
    setTicksToLive(max(100, 300 - 10 * world->currentLevelNum));
}

void SonarKit::move() {
    if (!isAlive()) { return; }

    if (getWorld()->isNearIceMan(this, 3.0)) {
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(75);
        getWorld()->player->addSonar();
    }

    else if (getTicksToLive() == 0) { setDead(); }

    decreaseTick();
}

WaterPool::WaterPool(StudentWorld* world, int startX, int startY)
    :Goodies(world, startX, startY, IID_WATER_POOL, SOUND_GOT_GOODIE, true, false, true)
{
    setTicksToLive(max(100, 300 - 10 * world->currentLevelNum));
}

void WaterPool::move() {
    if (!isAlive()) { return; }

    if (getWorld()->isNearIceMan(this, 3.0)) {
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(100);
        getWorld()->player->addWater();
    }

    else if (getTicksToLive() == 0) { setDead(); }

    decreaseTick();
}

Squirt::Squirt(StudentWorld* world, int startX, int startY, Direction startDir) 
    : Actor(world, startX, startY, startDir, IID_WATER_SPURT, 1, 1, true)
    ,distanceTraveled(0) {}

void Squirt::move() {
    if (isAlive() == false) {
        return;
    }

    //if actor is within a radius of 3 annoy the protestor by 2
    if (distanceTraveled > 4) {
        setDead();
        setVisible(false);
        return;
    }

    Direction dir = getDirection();
    int x = getX();
    int y = getY();
    if (getWorld()->scanIce(this, dir, 1) == true) {
        switch (dir) {
        case up:
            if (moveToIfPossible(x, y + 1))
                distanceTraveled++;
            else
                setDead();
            break;
        case down:
            if (moveToIfPossible(x, y - 1))
                distanceTraveled++;
            else
                setDead();
            break;
        case right:
            if (moveToIfPossible(x + 1, y))
                distanceTraveled++;
            else
                setDead();
            break;
        case left:
            if (moveToIfPossible(x - 1, y))
                distanceTraveled++;
            else
                setDead();
            break;

        }
    }
}