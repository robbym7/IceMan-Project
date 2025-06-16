#include "Actor.h"
#include "StudentWorld.h"


#include <vector>
#include <queue>

using namespace std;

Actor::Actor(StudentWorld* sw, int startX, int startY, Direction startDir, int imageID, double size, int depth, bool visible)
    :GraphObject(imageID, startX, startY, startDir, size, depth)
    , alive(true), world(sw)
{
    if (visible)
        setVisible(true);
    else
        setVisible(false);
}
bool Actor::isAlive() const
{
    return true;
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
    //setVisible(true);
}
unsigned int Agent::getHitPoints() const
{
    return hp;
}

//reduce hp by the annoyed amount, if hp is less then 0 it kills the agent

/*
bool Agent::annoy(unsigned int amount)
{
    hp -= amount;
    if (hp <= 0)
        setDead();
    return false;
}
*/



//=========================== ICEMAN ==============================
IceMan::IceMan(StudentWorld* world, int startX, int startY) :
    Agent(world, startX, startY, right, IID_PLAYER, 10), waterCharge(5)
{
    setVisible(true);
}

void IceMan::move() {
    int input;
    int playerX = getX();
	int playerY = getY();
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
            case KEY_PRESS_SPACE:
            if (waterCharge <= 0)
				return;
			if (waterCharge > 0) {
				sw->addActor(new Squirt(sw, playerX, playerY, getDirection()));
				waterCharge--;
			}
			break;
        }
        

    }
}

//=========================== ICE ==============================
Ice::Ice(StudentWorld* sw, int startX, int startY)
    :Actor(sw, startX, startY, right, IID_ICE, 0.25, 3, true)
{
    setVisible(true);
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

//bool Protester::annoy(unsigned int amount)
//{
//    return false;
//}


// bool Protester::huntsIceMan() const
// {
//     return true;
// }

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
    //setVisible(true);
}
void RegularProtester::move() {
    //check if alive
    if (isAlive() == false) {
        return;
    }
    //check if in rest state
    if (tickCounter % getTicksToNextMove() != 0) {
        increaseTick();
        
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
                getWorld()->icefield;
                int currentX= getX();
                int currentY= getY();
                //move one step towards the exit
                //pathFind(currentX, currentY, getWorld()->icefield);
                
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
            int newX = x + i;
            int newY = y + j;
            if (!isValidCell(newX, newY) || icefield[newX][newY]) return false;
        }
    }
    return true;
}

void RegularProtester::pathFind(int& x, int& y, Ice* icefield[VIEW_WIDTH][VIEW_HEIGHT]) {
    vector<vector<int>> dist(64, vector<int>(64, -1));
    queue<pair<int, int>> q;

    if (!isPassable(60, 60, icefield)) return;

    dist[60][60] = 0;
    q.push({60, 60});

    while (!q.empty()) {
        int currentX = q.front().first;
        int currentY = q.front().second;
        q.pop();
        int currentDistance = dist[currentX][currentY];

        for (int dir = 0; dir < 4; ++dir) {
            int newX = currentX, newY = currentY;
            switch (dir) {
                case 0: newX = currentX + 1; break;
                case 1: newX = currentX - 1; break;
                case 2: newY = currentY + 1; break;
                case 3: newY = currentY - 1; break;
            }

            if (!isValidCell(newX, newY)) continue;
            if (dist[newX][newY] != -1) continue;
            if (!isPassable(newX, newY, icefield)) continue;

            dist[newX][newY] = currentDistance + 1;
            q.push({newX, newY});
        }
    }
    //case for if dist is unchanged(unreachable place)
    if (dist[x][y] == -1) {
        return;
    }
    

    int bestDist = dist[x][y];
    int bestX = x;
    int bestY = y;

    for (int dir = 0; dir < 4; ++dir) {
        int newX = x, newY = y;
        switch (dir) {
            case 0: newX = x + 1; break;
            case 1: newX = x - 1; break;
            case 2: newY = y + 1; break;
            case 3: newY = y - 1; break;
        }

        if (!isValidCell(newX, newY)) continue;
        if (dist[newX][newY] == -1) continue;
        if (dist[newX][newY] < bestDist) {
            bestDist = dist[newX][newY];
            bestX = newX;
            bestY = newY;
        }
    }

    //moveTo(bestX, bestY);
}
void addGold() {}


// =========================== HARDCORE PROTESTOR ==============================
HardcoreProtester::HardcoreProtester(StudentWorld* world, int startX, int startY, int imageID) :
    Protester(world, startX, startY, IID_PROTESTER, 5, 0) {

}
void HardcoreProtester::move() {
    //check if alive
   
//void HardcoreProtester::addGold(){}


}




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

// =========================== BOULDER ==============================
void Boulder::move()
{
	//Student World will deal with deleting the boulder;
	if (isAlive() == false)
		return;
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
			getWorld()->getPlayer()->setDead();
		}
		if (getX() != 0 && getWorld()->scanIce(this, down, 1) == true) {
			moveToIfPossible(getX(), getY() - 1);
		}
		else {
			setDead();
		}
	}
}

bool Boulder::canActorsPassThroughMe() const
{
	return false;
}
