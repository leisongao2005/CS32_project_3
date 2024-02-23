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
    virtual bool canPush(int direction, Actor* pusher) {return true;}
    virtual bool isPit() {return false;}
    virtual bool isAlive() {return m_isAlive;}
    virtual bool isExit() {return false;}
    virtual bool isMarble() {return false;}
    virtual bool getAttacked() {return false;}
    virtual bool isPeaObstacle() {return false;}
    virtual bool isWall() {return false;}
    virtual bool isGoodie() {return false;}
    virtual bool isPlayer() {return false;}
    virtual bool isThiefBot() {return false;}
    void setDead() {m_isAlive = false;}
    StudentWorld* getWorld() {return m_world;}
private:
    StudentWorld* m_world;
    bool m_isAlive;
};

class Pea : public Actor {
public:
    Pea(int startX, int startY, StudentWorld* world, int direction): Actor(IID_PEA, startX, startY, world, direction) {setVisible(true);}
    virtual void doSomething();
};
    
class Avatar : public Actor {
public:
    Avatar(int startX, int startY, StudentWorld* world): Actor(IID_PLAYER, startX, startY, world, right), m_hitpoints(20), m_numPeas(20) {setVisible(true);}
    virtual void doSomething();
    virtual bool getAttacked();
    virtual bool isPlayer() {return true;}
    void firePea();
    int getHealth() {return m_hitpoints*5;}
    int getAmmo() {return m_numPeas;}
    void maxHealth() {m_hitpoints = 20;}
    void addAmmo() {m_numPeas += 20;}
private:
    int m_hitpoints;
    int m_numPeas;
};

class Wall : public Actor {
public:
    Wall(int startX, int startY, StudentWorld* world): Actor(IID_WALL, startX, startY, world) {setVisible(true);}
    virtual bool canPush(int direction, Actor* pusher) {return false;}
    virtual bool isPeaObstacle() {return true;}
    virtual bool isWall() {return true;}
    virtual bool getAttacked() {return true;}
};

class Marble : public Actor {
public:
    Marble(int startX, int startY, StudentWorld* world): Actor(IID_MARBLE, startX, startY, world), m_hitpoints(10) {setVisible(true);}
    virtual bool getAttacked();
    void getPushed(int direction);
    virtual bool canPush(int direction, Actor* pusher);
    virtual bool isPeaObstacle() {return true;}
    virtual bool isMarble() {return true;}
private:
    int m_hitpoints;
};

class Pit : public Actor {
public:
    Pit(int startX, int startY, StudentWorld* world): Actor(IID_PIT, startX, startY, world) {setVisible(true);}
    virtual void doSomething();
    virtual bool canPush(int direction, Actor* pusher) {return false;}
    virtual bool isPit() {return true;}
};

class Crystal : public Actor {
public:
    Crystal(int startX, int startY, StudentWorld* world): Actor(IID_CRYSTAL, startX, startY, world) {setVisible(true);}
    virtual void doSomething();
};

class Exit : public Actor {
public:
    Exit(int startX, int startY, StudentWorld* world): Actor(IID_EXIT, startX, startY, world) {setVisible(false);}
    virtual void doSomething();
    virtual bool isExit() {return true;}
//private:
//    bool m_isVisible;
};

class Goodie : public Actor {
public:
    Goodie(int imageID, int startX, int startY, StudentWorld* world): Actor(imageID, startX, startY, world) {}
    virtual void doSomething()=0;
    virtual bool isGoodie() {return true;}
};

class ExtraLifeGoodie : public Goodie {
public:
    ExtraLifeGoodie(int startX, int startY, StudentWorld* world): Goodie(IID_EXTRA_LIFE, startX, startY, world) {setVisible(true);}
    virtual void doSomething();
};

class RestoreHealthGoodie : public Goodie {
public:
    RestoreHealthGoodie(int startX, int startY, StudentWorld* world): Goodie(IID_RESTORE_HEALTH, startX, startY, world) {setVisible(true);}
    virtual void doSomething();
};

class AmmoGoodie : public Goodie {
public:
    AmmoGoodie(int startX, int startY, StudentWorld* world): Goodie(IID_AMMO, startX, startY, world) {setVisible(true);}
    virtual void doSomething();
};

class Robot : public Actor {
public:
    Robot(int imageID, int startX, int startY, StudentWorld* world, int direction, int hp);
    virtual void doSomething()=0;
    virtual bool getAttacked()=0;
    void takeDamage(int damage) {m_hitpoints -= damage;}
    int getHealth() {return m_hitpoints;}
    virtual bool canPush(int direction, Actor* pusher) {return false;}
    bool checkTick() {
        if (m_curr_tick < m_ticks) {
            m_curr_tick ++;
            return false;
        }
        else {
            m_curr_tick = 0;
            return true;
        }
    }
    void firePea();
    virtual bool isPeaObstacle() {return true;}
private:
    int m_hitpoints;
    int m_ticks;
    int m_curr_tick;
};

class RageBot : public Robot {
public:
    RageBot(int startX, int startY, StudentWorld* world, int direction): Robot(IID_RAGEBOT, startX, startY, world, direction, 10) {setVisible(true);}
    virtual void doSomething();
    virtual bool getAttacked();
};

class ThiefBot : public Robot {
public:
    ThiefBot(int startX, int startY, StudentWorld* world, int imageID = IID_THIEFBOT, int hp = 5): Robot(imageID, startX, startY, world, right, hp), m_distanceBeforeTurning(randInt(1, 6)), m_Goodie(nullptr) {setVisible(true);}
    virtual void doSomething();
    virtual bool getAttacked();
    virtual bool isThiefBot() {return true;}
    void move();
    void setGoodie(Actor* goodie) {m_Goodie = goodie;}
    Actor* getGoodie() {return m_Goodie;}
    void setDistanceBeforeTurning(int dist) {m_distanceBeforeTurning = dist;}
    int getDistanceBeforeTurning() {return m_distanceBeforeTurning;}
private:
    int m_distanceBeforeTurning;
    Actor* m_Goodie;
};

class MeanThiefBot : public ThiefBot {
public:
    MeanThiefBot(int startX, int startY, StudentWorld* world): ThiefBot(startX, startY, world, IID_MEAN_THIEFBOT, 8) {setVisible(true);}
    virtual void doSomething();
    virtual bool getAttacked();
};

class ThiefBotFactory : public Actor {
public:
    ThiefBotFactory(int startX, int startY, bool regular, StudentWorld* world): Actor(IID_ROBOT_FACTORY, startX, startY, world) {
        if (regular)
            m_makeRegularBots = true;
        else
            m_makeRegularBots = false;
        setVisible(true);
    }
    virtual void doSomething();
    virtual bool canPush(int direction, Actor* pusher) {return false;}
    virtual bool getAttacked() {return true;}
    virtual bool isPeaObstacle() {return true;}
private:
    bool m_makeRegularBots;
};

#endif // ACTOR_H_
