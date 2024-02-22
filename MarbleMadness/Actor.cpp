#include "Actor.h"
#include "StudentWorld.h"

#include <iostream>
using namespace std;
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void Avatar::doSomething() {
    if (!isAlive())
        return;
    int ch;
    // must implement getWorld function
    if (getWorld()->getKey(ch)) {
        switch (ch)
        {
            case KEY_PRESS_ESCAPE:
                setDead();
                break;
            case KEY_PRESS_SPACE: // fires a pea
                firePea();
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                if (!getWorld()->isObstructed(getX(), getY() + 1, up))
                    moveTo(getX(), getY() + 1);
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                if (!getWorld()->isObstructed(getX(), getY() - 1, down))
                    moveTo(getX(), getY() - 1);
                break;
            case KEY_PRESS_LEFT:
                setDirection(left);
                if (!getWorld()->isObstructed(getX() - 1, getY(), left))
                    moveTo(getX() - 1, getY());
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                if (!getWorld()->isObstructed(getX() + 1, getY(), right))
                    moveTo(getX() + 1, getY());
                break;
        }
    }
}

void Avatar::firePea() {
    int dir = getDirection();
    if (m_numPeas <= 0)
        return;
    switch(dir) {
        case up:
            getWorld()->addObject(new Pea(getX(), getY() + 1, getWorld(), getDirection()));
            break;
        case down:
            getWorld()->addObject(new Pea(getX(), getY() - 1, getWorld(), getDirection()));
            break;
        case left:
            getWorld()->addObject(new Pea(getX() - 1, getY(), getWorld(), getDirection()));
            break;
        case right:
            getWorld()->addObject(new Pea(getX() + 1, getY(), getWorld(), getDirection()));
            break;
    }
    m_numPeas--;
}

bool Avatar::getAttacked() {
    m_hitpoints -= 2;
    if (m_hitpoints < 0) {
        getWorld()->playSound(SOUND_PLAYER_DIE);
        setDead();
    }
    else
        getWorld()->playSound(SOUND_PLAYER_IMPACT);
    return true;
}

bool Marble::getAttacked() {
    m_hitpoints -= 2;
    if (m_hitpoints < 0)
        setDead();
    return true;
}

bool Marble::canPush(int direction) {
    int next_x = -1, next_y = -1;
    switch (direction) {
        case up:
            next_x = getX();
            next_y = getY() + 1;
            break;
        case down:
            next_x = getX();
            next_y = getY() - 1;
            break;
        case left:
            next_x = getX() - 1;
            next_y = getY();
            break;
        case right:
            next_x = getX() + 1;
            next_y = getY();
            break;
    }
    if (getWorld()->isPit(next_x, next_y) || getWorld()->isEmpty(next_x, next_y)) {
        moveTo(next_x, next_y);
        return true;
    }
    return false;
}

void Pea::doSomething() {
    if (!isAlive())
        return;
    vector<Actor*> items = getWorld()->getActor(getX(), getY());
    for (size_t i = 0; i != items.size(); i++) {
        Actor* actor = items.at(i);
        if (actor->getAttacked()) {
            setDead();
        }
    }
    moveForward();
    items = getWorld()->getActor(getX(), getY());
    for (size_t i = 0; i != items.size(); i++) {
        Actor* actor = items.at(i);
        if (actor->getAttacked()) {
            setDead();
        }
    }
}

void Pit::doSomething() {
    if (!isAlive())
        return;
    vector<Actor*> items = getWorld()->getActor(getX(), getY());
    for (size_t i = 0; i != items.size(); i++) {
        Actor* actor = items.at(i);
        if (actor->isMarble()) {
            setDead();
            actor->setDead();
        }
    }
}

void Crystal::doSomething() {
    if (!isAlive())
        return;
    if (getWorld()->playerHere(getX(), getY())) {
        getWorld()->increaseScore(50);
        getWorld()->getCrystal();
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
}

void Exit::doSomething() {
    if (isVisible() && getWorld()->playerHere(getX(), getY())) {
        getWorld()->playSound(SOUND_FINISHED_LEVEL);
        getWorld()->completeLevel();
    }
}

