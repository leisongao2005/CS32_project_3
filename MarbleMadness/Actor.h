#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
class Agent;
class Actor : public GraphObject
{
public:
    Actor(StudentWorld* world, int startX, int startY, int imageID,
          int hitPoints, int startDir): GraphObject(imageID, startX, startY, startDir), m_world(world), m_hitpoints(hitPoints), m_alive(true) {}

      // Action to perform each tick
    virtual void doSomething() = 0;

      // Is this actor alive?
    bool isAlive() const {return m_alive;}
    
      // Mark this actor as dead
    void setDead() {m_alive = false;}
    
      // Reduce this actor's hit points
    void decHitPoints(int amt) {m_hitpoints -= amt;}
    
      // Get this actor's world
    StudentWorld* getWorld() const {return m_world;}
    
      // Can an agent occupy the same square as this actor?
    virtual bool allowsAgentColocation() const {return false;}
    
      // Can a marble occupy the same square as this actor?
    virtual bool allowsMarble() const {return false;}
    
      // Does this actor count when a factory counts items near it?
    virtual bool countsInFactoryCensus() const {return false;}
    
      // Does this actor stop peas from continuing?
    virtual bool stopsPea() const {return false;}
    
      // Can this actor be damaged by peas?
    virtual bool isDamageable() const {return false;}

      // Cause this Actor to sustain damageAmt hit points of damage.
    virtual void damage(int damageAmt) {m_hitpoints -= damageAmt;}
    
      // Can this actor be pushed by a to location x,y?
    virtual bool bePushedBy(Agent* a, int x, int y) {return false;}
    
      // Can this actor be swallowed by a pit?
    virtual bool isSwallowable() const {return false;}
    
      // Can this actor be picked up by a ThiefBot?
    virtual bool isStealable() const {return false;}
    
      // How many hit points does this actor have left?
    virtual int getHitPoints() const {return m_hitpoints;}
    
      // Set this actor's hit points to amt.
    virtual void setHitPoints(int amt) {m_hitpoints = amt;}
    
      // Make the actor sustain damage.  Return true if this kills the
      // actor, and false otherwise.
    virtual bool tryToBeKilled(int damageAmt) {
        m_hitpoints -= damageAmt;
        if (m_hitpoints <= 0) {
            m_alive = false;
            return true;
        }
        return false;
    }
    
    int getdx() const {
        int dx = 0;
        switch (getDirection()) {
            case left:
                dx = -1;
                break;
            case right:
                dx = 1;
                break;
        }
        return dx;
    }
    
    int getdy() const {
        int dy = 0;
        switch (getDirection()) {
            case up:
                dy = 1;
                break;
            case down:
                dy = -1;
                break;
        }
        return dy;
    }
    
    virtual bool isExit() const {return false;}

private:
    bool m_alive;
    int m_hitpoints;
    StudentWorld* m_world;
};

class Agent : public Actor
{
public:
    Agent(StudentWorld* world, int startX, int startY, int imageID,
          int hitPoints, int startDir): Actor(world, startX, startY, imageID, hitPoints, startDir) {}

      // Move to the adjacent square in the direction the agent is facing
      // if it is not blocked, and return true.  Return false if the agent
      // can't move.
    bool moveIfPossible();

      // Return true if this agent can push marbles (which means it's the
      // player).
    virtual bool canPushMarbles() const {return false;}

      // Return true if this agent doesn't shoot unless there's an unobstructed
      // path to the player.
    
    //*** should only be the player who can shoot without clear path
    virtual bool needsClearShot() const {return true;}

      // Return the sound effect ID for a shot from this agent.
    virtual int shootingSound() const {return SOUND_ENEMY_FIRE;}
};

class Player : public Agent
{
public:
    Player(StudentWorld* world, int startX, int startY): Agent(world, startX, startY, IID_PLAYER, 20, right), m_ammo(20) {setVisible(true);}
    
    virtual void doSomething();
    virtual bool isDamageable() const {return true;}
    virtual void damage(int damageAmt);
    virtual bool canPushMarbles() const {return true;}
    virtual bool needsClearShot() const {return false;}
    virtual int shootingSound() const {return SOUND_PLAYER_FIRE;}
    
      // Get player's health percentage
    int getHealthPct() const {
        return getHitPoints() * 5;
    }

      // Get player's amount of ammunition
    int getAmmo() const {
        return m_ammo;
    };
    
      // Restore player's health to the full amount.
    void restoreHealth() {
        setHitPoints(20);
    }
    
      // Increase player's amount of ammunition.
    void increaseAmmo() {
        m_ammo += 20;
    }
    
private:
//    void firePea() const;
    int m_ammo;
};


class Robot : public Agent
{
public:
    Robot(StudentWorld* world, int startX, int startY, int imageID,
          int hitPoints, int score, int startDir);
    virtual void doSomething();
    virtual bool isDamageable() const {return true;}
    virtual void damage(int damageAmt);
    virtual bool canPushMarbles() const {return false;}
    virtual bool needsClearShot() const {return true;}
    virtual int shootingSound() const {return SOUND_ENEMY_FIRE;}
    virtual void dieForSpecificRobotSubclass() {} ;
    virtual void doSomethingForSpecificRobotSubclass() {} ;


      // Does this robot shoot?
    virtual bool isShootingRobot() const {return true;}
    
private:
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
    int m_score;
    int m_ticks;
    int m_curr_tick;
};

class RageBot : public Robot
{
public:
    RageBot(StudentWorld* world, int startX, int startY, int startDir): Robot(world, startX, startY, IID_RAGEBOT, 10, 100, startDir) {}
//    virtual void doSomething();
    virtual void dieForSpecificRobotSubclass() {};
    virtual void doSomethingForSpecificRobotSubclass();
};

class ThiefBot : public Robot
{
public:
    ThiefBot(StudentWorld* world, int startX, int startY, int imageID,
             int hitPoints, int score): Robot(world, startX, startY, imageID, hitPoints, score, right), m_Goodie(nullptr), m_distanceBeforeTurning(randInt(1, 6)) {}
//    virtual void doSomething();
    virtual bool countsInFactoryCensus() const {return true;}
//    virtual void damage(int damageAmt);
    // drop goodie
    virtual void dieForSpecificRobotSubclass();
    virtual void doSomethingForSpecificRobotSubclass();
private:
    int m_distanceBeforeTurning;
    Actor* m_Goodie;
};
class RegularThiefBot : public ThiefBot
{
public:
    RegularThiefBot(StudentWorld* world, int startX, int startY): ThiefBot(world, startX, startY, IID_THIEFBOT, 5, 10) {}
//    virtual void doSomething();
    virtual bool isShootingRobot() const {return false;}
};

class MeanThiefBot : public ThiefBot
{
public:
    MeanThiefBot(StudentWorld* world, int startX, int startY): ThiefBot(world, startX, startY, IID_MEAN_THIEFBOT, 8, 20) {}
//    virtual void doSomething();
};

class Exit : public Actor
{
public:
    Exit(StudentWorld* world, int startX, int startY): Actor(world, startX, startY, IID_EXIT, 0, none) {setVisible(false);}
    virtual void doSomething();
    virtual bool allowsAgentColocation() const {return true;}
    virtual bool isExit() const {return true;}
};

class Wall : public Actor
{
public:
    Wall(StudentWorld* world, int startX, int startY): Actor(world, startX, startY, IID_WALL, 0, none) {setVisible(true);}
    virtual void doSomething() {};
    virtual bool stopsPea() const {return true;}
};

class Marble : public Actor
{
public:
    Marble(StudentWorld* world, int startX, int startY): Actor(world, startX, startY, IID_MARBLE, 10, none) {setVisible(true);}
    virtual void doSomething() {}
    virtual bool isDamageable() const {return true;}
    virtual void damage(int damageAmt) {
        if (getHitPoints() > damageAmt) {
            decHitPoints(damageAmt);
        }
        else {
            setDead();
        }
    }
    virtual bool isSwallowable() const {return true;}
    virtual bool bePushedBy(Agent* a, int x, int y);
};

class Pit : public Actor
{
public:
    Pit(StudentWorld* world, int startX, int startY): Actor(world, startX, startY, IID_PIT, 0, none) {setVisible(true);}
    virtual void doSomething();
    virtual bool allowsMarble() const {return true;}
};

class Pea : public Actor
{
public:
    Pea(StudentWorld* world, int startX, int startY, int startDir): Actor(world, startX, startY, IID_PEA, 0, startDir) {setVisible(true);}
    virtual void doSomething();
    virtual bool allowsAgentColocation() const {return true;}
};

class ThiefBotFactory : public Actor
{
public:
    ThiefBotFactory(StudentWorld* world, int startX, int startY, bool type): Actor(world, startX, startY, IID_ROBOT_FACTORY, 0, none), m_meanBots(type) {setVisible(true);}
    virtual void doSomething();
    virtual bool stopsPea() const {return true;}
private:
    bool m_meanBots;
};

class PickupableItem : public Actor
{
public:
    PickupableItem(StudentWorld* world, int startX, int startY, int imageID,
                   int score): Actor(world, startX, startY, imageID, 0, none), m_score(score) {}
    virtual void doSomething();
    virtual bool allowsAgentColocation() const {return true;}
    virtual void doItemSpecificSomething() = 0;
private:
    int m_score;
};

class Crystal : public PickupableItem
{
public:
    Crystal(StudentWorld* world, int startX, int startY): PickupableItem(world, startX, startY, IID_CRYSTAL, 50) {setVisible(true);}
    virtual void doItemSpecificSomething();
};

class Goodie : public PickupableItem
{
public:
    Goodie(StudentWorld* world, int startX, int startY, int imageID,
           int score): PickupableItem(world, startX, startY, imageID, score), m_stolen(false) {}
    virtual bool isStealable() const {return true;}
    virtual void doItemSpecificSomething();
    virtual void doGoodieSpecificSomething()=0;

      // Set whether this goodie is currently stolen.
    void setStolen(bool status) {m_stolen = status;}
private:
    bool m_stolen;
};

class ExtraLifeGoodie : public Goodie
{
public:
    ExtraLifeGoodie(StudentWorld* world, int startX, int startY): Goodie(world, startX, startY, IID_EXTRA_LIFE, 1000) {}
    virtual void doGoodieSpecificSomething();
};

class RestoreHealthGoodie : public Goodie
{
public:
    RestoreHealthGoodie(StudentWorld* world, int startX, int startY): Goodie(world, startX, startY, IID_RESTORE_HEALTH, 500) {}
    virtual void doGoodieSpecificSomething();
};

class AmmoGoodie : public Goodie
{
public:
    AmmoGoodie(StudentWorld* world, int startX, int startY): Goodie(world, startX, startY, IID_AMMO, 100) {setVisible(true);}
    virtual void doGoodieSpecificSomething();
};

#endif // ACTOR_H_
