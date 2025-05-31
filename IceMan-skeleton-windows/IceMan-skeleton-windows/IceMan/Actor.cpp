#include "Actor.h"
#include "StudentWorld.h"

//TODO: for now all of these are dummy functions so that StudentWorld "works"

//constructor for actor
Actor::Actor(StudentWorld* sw, int startX, int startY, Direction startDir, int imageID, double size, int depth, bool visible)
    :GraphObject(imageID, startX, startY, startDir, size, depth)
    ,alive(true), world(sw)
{
    setVisible(visible);
}

void Actor::setAlive(bool state) { alive = state; }

IceMan::IceMan(StudentWorld* sw, int startX, int startY)
    :Actor(sw, startX, startY, right, IID_PLAYER, 1.0, 0, true)
{
    setVisible(true);
}

void IceMan::doSomething() {

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
                moveTo(getX() - 1, getY());
                getWorld()->playerDig(GraphObject::left, getX(), getY());
            }
            break;
        case KEY_PRESS_RIGHT:
            if (dir != right) {
                setDirection(right);
            }
            else {
                moveTo(getX() + 1, getY());
                getWorld()->playerDig(GraphObject::right, getX(), getY());
            }
            break;
        case KEY_PRESS_UP:
            if (dir != up) {
                setDirection(up);
            }
            else {
                moveTo(getX(), getY() + 1);
                getWorld()->playerDig(GraphObject::up, getX(), getY());

            }
            break;
        case KEY_PRESS_DOWN:
            if (dir != down) {
                setDirection(down);
            }
            else {
                moveTo(getX(), getY() - 1);
                getWorld()->playerDig(GraphObject::down, getX(), getY());

            }
            break;
        }
    }
}

Ice::Ice(StudentWorld* sw, int startX, int startY)
    :Actor(sw, startX, startY, right, IID_ICE, 0.25, 3, true)
{
    setVisible(true);
}

void Ice::doSomething() { ; };

/*
void Boulders:: move(){};

void Squirt:: move(){};

void OilBarrel:: move(){};

void GoldNugget:: move(){};

void SonarKit:: move(){};

void WaterPool:: move(){};
*/

bool Actor::isAlive() const
{
    return alive;
}

StudentWorld* Actor::getWorld() const
{
    return world;
}