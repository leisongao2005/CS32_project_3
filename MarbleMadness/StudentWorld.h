#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Avatar;
class Actor;
//class GraphObject;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void getCrystal() {m_crystalsCollected++;}
    void addObject(Actor* object);
    bool isObstructed(int x, int y, int direction);
    bool isPit(int x, int y);
    bool isEmpty(int x, int y);

private:
    Avatar* m_player;
    std::vector<Actor*> m_Actors;
    int m_crystals;
    int m_crystalsCollected;
    int m_exitX;
    int m_exitY;
    int m_bonus;
};

#endif // STUDENTWORLD_H_
