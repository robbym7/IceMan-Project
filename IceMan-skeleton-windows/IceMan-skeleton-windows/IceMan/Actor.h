#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

//TODO: for now all of these are dummy functions so that StudentWorld "works"

class Actor : public GraphObject {
public:
    Actor();
    //gets player_alive
    bool isAlive() const;


private:
    bool alive;



};

class Protester: public Actor {
public:
};
class HardcoreProtester: public Protester {
public:
};


#endif // ACTOR_H_
