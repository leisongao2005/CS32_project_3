#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject {
public:
    Actor(int ID, int startX, int startY, StudentWorld* world, int direction = none): GraphObject(ID, startX, startY, direction), m_isAlive(true), m_world(world) {}
    virtual void doSomething() {};
    virtual bool canPush(int direction) {return true;}
    virtual bool isPit() {return false;}
    virtual bool isAlive() {return m_isAlive;}
    virtual bool isExit() {return false;}
    StudentWorld* getWorld() {return m_world;}
private:
    StudentWorld* m_world;
    bool m_isAlive;
};

class Pea : public Actor {
public:
    Pea(int startX, int startY, StudentWorld* world, int direction): Actor(IID_PEA, startX, startY, world, direction) {}
    virtual void doSomething() {};
private:
    bool m_isAlive;
};
    
class Avatar : public Actor {
public:
    Avatar(int startX, int startY, StudentWorld* world): Actor(IID_PLAYER, startX, startY, world, right), m_hitpoints(20), m_numPeas(20), m_isAlive(true) {
        setVisible(true);
    }
    bool isAlive() {return m_isAlive;}
    virtual void doSomething();
    void getAttacked();
    void firePea();
    int getHealth() {return m_hitpoints*5;}
    int getAmmo() {return m_numPeas;}
private:
    int m_hitpoints;
    int m_numPeas;
    bool m_isAlive;
};

class Wall : public Actor {
public:
    Wall(int startX, int startY, StudentWorld* world): Actor(IID_WALL, startX, startY, world) {setVisible(true);}
    virtual bool canPush(int direction) {return false;}
};

class Marble : public Actor {
public:
    Marble(int startX, int startY, StudentWorld* world): Actor(IID_MARBLE, startX, startY, world), m_hitpoints(10), m_isAlive(true){}
    void getAttacked();
    void getPushed(int direction);
    virtual bool canPush(int direction);
private:
    int m_hitpoints;
    bool m_isAlive;
};

class Pit : public Actor {
public:
    Pit(int startX, int startY, StudentWorld* world): Actor(IID_PIT, startX, startY, world) {}
    virtual void doSomething() {};
    virtual bool canPush(int direction) {return false;}
    virtual bool isPit() {return true;}
private:
    bool m_isAlive;
};

class Crystal : public Actor {
public:
    Crystal(int startX, int startY, StudentWorld* world): Actor(IID_CRYSTAL, startX, startY, world) {}
    virtual void doSomething() {};
private:
    bool m_isAlive;
};

class Exit : public Actor {
public:
    Exit(int startX, int startY, StudentWorld* world): Actor(IID_EXIT, startX, startY, world) {setVisible(false);}
    virtual void doSomething() {};
    virtual bool isExit() {return true;}
private:
    bool m_isVisible;
};

class Goodie : public Actor {
public:
    Goodie(int imageID, int startX, int startY, StudentWorld* world): Actor(imageID, startX, startY, world) {}
    virtual void doSomething()=0;
private:
    bool m_isAlive;
};

class ExtraLifeGoodie : public Goodie {
public:
    ExtraLifeGoodie(int startX, int startY, StudentWorld* world): Goodie(IID_EXTRA_LIFE, startX, startY, world) {}
    virtual void doSomething() {};
};

class RestoreHealthGoodie : public Goodie {
public:
    RestoreHealthGoodie(int startX, int startY, StudentWorld* world): Goodie(IID_RESTORE_HEALTH, startX, startY, world)  {}
    virtual void doSomething() {};
};

class AmmoGoodie : public Goodie {
public:
    AmmoGoodie(int startX, int startY, StudentWorld* world): Goodie(IID_AMMO, startX, startY, world)  {}
    virtual void doSomething() {};
};

class Robot : public Actor {
public:
    Robot(int imageID, int startX, int startY, StudentWorld* world, int direction): Actor(imageID, startX, startY, world, direction) {}
    virtual void doSomething()=0;
    virtual void getAttacked()=0;
    virtual bool canPush(int direction) {return false;}
private:
    int m_hitpoints;
    int m_ticks;
    bool m_isAlive;
};

class RageBot : public Robot {
public:
    RageBot(int startX, int startY, StudentWorld* world, int direction): Robot(IID_RAGEBOT, startX, startY, world, direction) {}
    virtual void doSomething() {};
    virtual void getAttacked() {};
};

class ThiefBot : public Robot {
public:
    ThiefBot(int startX, int startY, StudentWorld* world, int direction, int imageID = IID_THIEFBOT): Robot(imageID, startX, startY, world, direction) {}
    virtual void doSomething() {};
    virtual void getAttacked() {};
private:
    int m_distanceBeforeTurning;
};

class MeanThiefBot : public ThiefBot {
public:
    MeanThiefBot(int startX, int startY, StudentWorld* world, int direction): ThiefBot(startX, startY, world, direction, IID_MEAN_THIEFBOT) {}
    virtual void doSomething() {};
    virtual void getAttacked() {};
};

class ThiefBotFactory : public Actor {
public:
    ThiefBotFactory(int startX, int startY, bool regular, StudentWorld* world): Actor(IID_ROBOT_FACTORY, startX, startY, world) {
        if (regular)
            m_makeRegularBots = true;
        else
            m_makeRegularBots = false;
    }
    virtual void doSomething() {};
    virtual bool canPush(int direction) {return false;}
private:
    bool m_makeRegularBots;
};

#endif // ACTOR_H_
