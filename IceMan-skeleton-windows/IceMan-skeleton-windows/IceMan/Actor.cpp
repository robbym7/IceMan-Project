#include "Actor.h"
#include "StudentWorld.h"


#include <vector>
#include <queue>

using namespace std;

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

void Actor::setDead()
{
    alive = false;
}

StudentWorld* Actor::getWorld() const
{
    return world;
}

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



//=========================== AGENT ==============================

Agent::Agent(StudentWorld* world, int startX, int startY, Direction startDir, int imageID, unsigned int hitPoints) :
    Actor(world, startX, startY, startDir, true, imageID, 1.0, 0),
    hp(hitPoints)
{

}
unsigned int Agent::getHitPoints() const
{
    return hp;
}

//reduce hp by the annoyed amount, if hp is less then 0 it kills the agent
bool Agent::annoy(unsigned int amount)
{
    hp -= amount;
    if (hp <= 0)
        setDead();
    return false;
}


// IceMan::IceMan(StudentWorld* sw, int startX, int startY)
//     :Actor(sw, startX, startY, right, IID_PLAYER, 1.0, 0, true)
// {
//     setVisible(true);
// }


//=========================== ICEMAN ==============================
void IceMan::move() {
    /*if (isAlive() == false) {
        return;
    }*/

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
                    sw->playerDig(GraphObject::left, getX(), getY());
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
                    sw->playerDig(GraphObject::right, getX(), getY());
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
                    sw->playerDig(GraphObject::up, getX(), getY());
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
                    sw->playerDig(GraphObject::down, getX(), getY());
                }
            }
            break;
        }
    }
}

//=========================== ICE ==============================
Ice::Ice(StudentWorld* world, int startX, int startY) : Actor(world, startX, startY, right, true, IID_ICE, 0.25, 3)
{
}
void Ice::move() {}

//=========================== PROTESTOR CLASS ==============================
Protester::Protester(StudentWorld* world, int startX, int startY, int imageID, unsigned int hitPoints, unsigned int score) :
    Agent(world, startX, startY, left, imageID, hitPoints)

{
    setTicksToNextMove();
}

void Protester::move()
{
}

bool Protester::annoy(unsigned int amount)
{
    return false;
}

void Protester::addGold()
{
}

bool Protester::huntsIceMan() const
{
    return true;
}

void Protester::setTicksToNextMove()
{
    ticksBetweenMoves = (std::max(0, 3 - (int)(getWorld()->getLevel()) / 4));
}

void Protester::setMustLeaveOilField()
{
    wantsToLeaveOilField = true;
}

bool Protester::canPickDroppedGoldUp() const
{
    return false;
}

int Protester::getTicksToNextMove() const
{
    return ticksBetweenMoves;
}

bool Protester::getWantsToLeaveOilField() const
{
    return wantsToLeaveOilField;
}

void Protester::incNonRestingTick()
{
    nonRestingTick++;
}

int Protester::getNonRestingTick() const
{
    return nonRestingTick;
}

void Protester::resetNonRestingTick()
{
    nonRestingTick = 0;
}







//=========================== REGULAR PROTESTOR ==============================
RegularProtester::RegularProtester(StudentWorld* world, int startX, int startY, int imageID) :
    Protester(world, startX, startY, IID_PROTESTER, 5, 0) {
    setTicksToNextMove();
    setVisible(true);
}
void RegularProtester::move() {
    //check if alive
    if (isAlive() == false) {
        return;
    }
    //check if in rest state
    if (tickCounter % getTicksToNextMove() != 0) {
        increaseTick();
        return;
    }
    else {
        //check if it wants to leave oil field
        if (getWantsToLeaveOilField() == true) {
            // check if it is at its exit point
            if (getX() == VIEW_WIDTH - SPRITE_WIDTH && getY() == VIEW_HEIGHT - SPRITE_HEIGHT) {
                setVisible(false);
                setDead();
                return;
            }
            //move towards the exit
            else {
                Ice* (*icefield)[VIEW_WIDTH] = getWorld()->getIceField();
                vector<vector<int>> dist = pathFind(icefield);

                int x = getX();
                int y = getY();
                int bestX = x;
                int bestY = y;
                int bestDist = dist[x][y];

                // Try each of the 4 directions
                const int dx[] = { 0, 0, -1, 1 };
                const int dy[] = { 1, -1, 0, 0 };
                for (int dir = 0; dir < 4; ++dir) {
                    int nx = x + dx[dir];
                    int ny = y + dy[dir];
                    if (nx < 0 || nx >= 64 || ny < 0 || ny >= 64)
                        continue;
                    if (dist[nx][ny] >= 0 && dist[nx][ny] < bestDist) {
                        bestDist = dist[nx][ny];
                        bestX = nx;
                        bestY = ny;
                    }
                }

                if (bestX != x || bestY != y) {
                    moveTo(bestX, bestY);
                }
            }
        }
    }
    if (getNonRestingTick() < 15) {
        incNonRestingTick();
    }
    if (getWorld()->isNearIceMan(this, 4) && getNonRestingTick() >= 15) {
        getWorld()->playSound(SOUND_PROTESTER_YELL);
        //getWorld()->annoyIceMan();
        resetNonRestingTick();
    }
    return;
}



// PATH FINDING FOR PROTESTORS
bool isValidCell(int x, int y) {
    return x >= 0 && y >= 0 && x < VIEW_WIDTH && y < VIEW_HEIGHT;
}
bool isPassable(int x, int y, Ice* icefield[VIEW_WIDTH][VIEW_HEIGHT]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            int nx = x + i;
            int ny = y + j;
            if (!isValidCell(nx, ny) || icefield[nx][ny]) return false;
        }
    }
    return true;
}
vector<vector<int>> pathFind(Ice* icefield[VIEW_WIDTH][VIEW_HEIGHT]) {
    vector<vector<int>> dist(64, vector<int>(64, -1));
    queue<pair<int, int>> q;

    if (isPassable(60, 60, icefield) == false) return dist;

    dist[60][60] = 0;
    q.push({ 60, 60 });

    while (!q.empty()) {
        int x = q.front().first;
        int y = q.front().second;
        q.pop();
        int currentDistance = dist[x][y];

        for (int dir = 0; dir < 4; dir++) {
            int nx, ny;

            if (dir == 0) { // right
                nx = x + 1;
                ny = y;
            }
            else if (dir == 1) { // left
                nx = x - 1;
                ny = y;
            }
            else if (dir == 2) { // up
                nx = x;
                ny = y + 1;
            }
            else { // down
                nx = x;
                ny = y - 1;
            }

            if (!isValidCell(nx, ny)) continue;
            if (dist[nx][ny] != -1) continue;
            if (!isPassable(nx, ny, icefield)) continue;

            dist[nx][ny] = currentDistance + 1;
            q.push({ nx, ny });
        }
    }

    return dist;
}
void addGold() {}


// =========================== HARDCORE PROTESTOR ==============================
HardcoreProtester::HardcoreProtester(StudentWorld* world, int startX, int startY, int imageID) :
    Protester(world, startX, startY, IID_PROTESTER, 5, 0) {

}
void HardcoreProtester::move() {
    //check if alive
    if (isAlive() == false) {
        return;
    }
    //check if in rest state
    if (tickCounter % getTicksToNextMove() != 0) {
        increaseTick();
        return;
    }
    else {
        //check if it wants to leave oil field
        if (getWantsToLeaveOilField() == true) {
            // check if it is at its exit point
            if (getX() == VIEW_WIDTH - SPRITE_WIDTH && getY() == VIEW_HEIGHT - SPRITE_HEIGHT) {
                setVisible(false);
                setDead();
                return;
            }
            //move towards the exit
            else {
                Ice* (*icefield)[VIEW_WIDTH] = getWorld()->getIceField();
                vector<vector<int>> dist = pathFind(icefield);

                int x = getX();
                int y = getY();
                int bestX = x;
                int bestY = y;
                int bestDist = dist[x][y];

                // Try each of the 4 directions
                const int dx[] = { 0, 0, -1, 1 };
                const int dy[] = { 1, -1, 0, 0 };
                for (int dir = 0; dir < 4; ++dir) {
                    int nx = x + dx[dir];
                    int ny = y + dy[dir];
                    if (nx < 0 || nx >= 64 || ny < 0 || ny >= 64)
                        continue;
                    if (dist[nx][ny] >= 0 && dist[nx][ny] < bestDist) {
                        bestDist = dist[nx][ny];
                        bestX = nx;
                        bestY = ny;
                    }
                }

                if (bestX != x || bestY != y) {
                    moveTo(bestX, bestY);
                }
            }
        }
    }
    if (getNonRestingTick() < 15) {
        incNonRestingTick();
    }
    if (getWorld()->isNearIceMan(this, 4) && getNonRestingTick() >= 15) {
        getWorld()->playSound(SOUND_PROTESTER_YELL);
        //getWorld()->annoyIceMan();
        resetNonRestingTick();
    }
    else {
        //check if iceman is in a straight line (not within 4 units, pathfinding works)
        //if he is then turn and move towards him 
    }
    return;
}
//void HardcoreProtester::addGold(){}







// =========================== SQUIRT ==============================
Squirt::Squirt(StudentWorld* world, int startX, int startY, Direction startDir) :
    Actor(world, startX, startY, startDir, true, IID_WATER_SPURT, 1, 1),
    distanceTraveled(0)
{
}
void Squirt::move()
{
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
    //if (getWorld()->scanIce(this, dir, 1) == true) {
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