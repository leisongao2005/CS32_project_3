#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor : public GraphObject {
public:
    Actor(int ID, int startX, int startY, int direction = none): GraphObject(ID, startX, startY, direction) {}
    virtual void doSomething() {};
    StudentWorld* getWorld() {return m_world;}
private:
    StudentWorld* m_world;
};
    
    
class Avatar : public Actor {
public:
    Avatar(int startX, int startY): Actor(IID_PLAYER, startX, startY, right), m_hitpoints(20), m_numPeas(20), m_isAlive(true) {
        setVisible(true);
    }
    bool isAlive() {return m_isAlive;}
    virtual void doSomething() {};
    void getAttacked();
    void firePea();
private:
    int m_hitpoints;
    int m_numPeas;
    bool m_isAlive;
};

class Wall : public Actor {
public:
    Wall(int startX, int startY): Actor(IID_WALL, startX, startY) {}
};

class Marble : public Actor {
public:
    Marble(int startX, int startY): Actor(IID_MARBLE, startX, startY) {}
    void getAttacked();
    void getPushed();
private:
    int m_hitpoints;
    bool m_isAlive;
};

class Pea : public Actor {
public:
    Pea(int startX, int startY, int direction): Actor(IID_PEA, startX, startY, direction) {}
    virtual void doSomething() {};
private:
    bool m_isAlive;
};

class Pit : public Actor {
public:
    Pit(int startX, int startY): Actor(IID_PIT, startX, startY) {}
    virtual void doSomething() {};
private:
    bool m_isAlive;
};

class Crystal : public Actor {
public:
    Crystal(int startX, int startY): Actor(IID_CRYSTAL, startX, startY) {}
    virtual void doSomething() {};
private:
    bool m_isAlive;
};

class Exit : public Actor {
public:
    Exit(int startX, int startY): Actor(IID_EXIT, startX, startY) {}
    virtual void doSomething() {};
private:
    bool m_isVisible;
};

class Goodie : public Actor {
public:
    Goodie(int imageID, int startX, int startY): Actor(imageID, startX, startY) {}
    virtual void doSomething()=0;
private:
    bool m_isAlive;
};

class ExtraLifeGoodie : public Goodie {
public:
    ExtraLifeGoodie(int startX, int startY): Goodie(IID_EXTRA_LIFE, startX, startY) {}
    virtual void doSomething() {};
};

class RestoreHealthGoodie : public Goodie {
public:
    RestoreHealthGoodie(int startX, int startY): Goodie(IID_RESTORE_HEALTH, startX, startY)  {}
    virtual void doSomething() {};
};

class AmmoGoodie : public Goodie {
public:
    AmmoGoodie(int startX, int startY): Goodie(IID_AMMO, startX, startY)  {}
    virtual void doSomething() {};
};

class Robot : public Actor {
public:
    Robot(int imageID, int startX, int startY, int direction): Actor(imageID, startX, startY, direction) {}
    virtual void doSomething()=0;
    virtual void getAttacked()=0;
private:
    int m_hitpoints;
    int m_ticks;
    bool m_isAlive;
};

class RageBot : public Robot {
public:
    RageBot(int startX, int startY, int direction): Robot(IID_RAGEBOT, startX, startY, direction) {}
    virtual void doSomething() {};
    virtual void getAttacked() {};
};

class ThiefBot : public Robot {
public:
    ThiefBot(int startX, int startY, int direction, int imageID = IID_THIEFBOT): Robot(imageID, startX, startY, direction) {}
    virtual void doSomething() {};
    virtual void getAttacked() {};
private:
    int m_distanceBeforeTurning;
};

class MeanThiefBot : public ThiefBot {
public:
    MeanThiefBot(int startX, int startY, int direction): ThiefBot(startX, startY, direction, IID_MEAN_THIEFBOT) {}
    virtual void doSomething() {};
    virtual void getAttacked() {};
};

class ThiefBotFactory : public Actor {
public:
    ThiefBotFactory(int startX, int startY, bool regular): Actor(IID_ROBOT_FACTORY, startX, startY) {
        if (regular)
            m_makeRegularBots = true;
        else
            m_makeRegularBots = false;
    }
    virtual void doSomething() {};
private:
    bool m_makeRegularBots;
};

#endif // ACTOR_H_
