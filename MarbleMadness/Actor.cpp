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
        else if (getWorld()->playerHere(getX(), getY())){
            getWorld()->getPlayer()->getAttacked();
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
        else if (getWorld()->playerHere(getX(), getY())){
            getWorld()->getPlayer()->getAttacked();
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

void ExtraLifeGoodie::doSomething() {
    if(!isAlive() || !isVisible())
        return;
    if (getWorld()->playerHere(getX(), getY())) {
        getWorld()->increaseScore(1000);
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->incLives();
    }
}

void RestoreHealthGoodie::doSomething() {
    if(!isAlive() || !isVisible())
        return;
    if (getWorld()->playerHere(getX(), getY())) {
        getWorld()->increaseScore(500);
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->restoreHealth();
    }
}

void AmmoGoodie::doSomething() {
    if(!isAlive() || !isVisible())
        return;
    if (getWorld()->playerHere(getX(), getY())) {
        getWorld()->increaseScore(100);
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->restoreAmmo();
    }
}
Robot::Robot(int imageID, int startX, int startY, StudentWorld* world, int direction, int hp): Actor(imageID, startX, startY, world, direction) {
    m_ticks = (28 - getWorld()->getLevel())/4;
    if (m_ticks < 3)
        m_ticks = 3;
    m_hitpoints = hp;
}

void Robot::firePea() {
    int dir = getDirection();
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
}

void RageBot::doSomething() {
    if (!isAlive())
        return;
    if (checkTick()) {
        if (getWorld()->playerInSight(getX(), getY(), getDirection())) {
            firePea();
        }
        else {
            switch(getDirection()) {
                case up:
                    if (!getWorld()->isObstructed(getX(), getY() + 1, up))
                        moveTo(getX(), getY() + 1);
                    else
                        setDirection(getDirection() + 180);
                    break;
                case down:
                    if (!getWorld()->isObstructed(getX(), getY() - 1, down))
                        moveTo(getX(), getY() - 1);
                    else
                        setDirection(getDirection() + 180);
                    break;
                case left:
                    if (!getWorld()->isObstructed(getX() - 1, getY(), left))
                        moveTo(getX() - 1, getY());
                    else
                        setDirection(getDirection() + 180);
                    break;
                case right:
                    if (!getWorld()->isObstructed(getX() + 1, getY(), right))
                        moveTo(getX() + 1, getY());
                    else
                        setDirection(getDirection() + 180);
                    break;
            }
        }
    }
}

bool RageBot::getAttacked() {
    takeDamage(2);
    if (getHealth() <= 0) {
        setDead();
        getWorld()->playSound(SOUND_ROBOT_DIE);
        getWorld()->increaseScore(100);
    }
    else {
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
    }
    return true;
}

void ThiefBot::move() {
    switch(getDirection()) {
        case up:
            if (!getWorld()->isObstructed(getX(), getY() + 1, up)) {
                moveTo(getX(), getY() + 1);
                return;
            }
        case down:
            if (!getWorld()->isObstructed(getX(), getY() - 1, down)) {
                moveTo(getX(), getY() - 1);
                return;
            }
        case left:
            if (!getWorld()->isObstructed(getX() - 1, getY(), left)) {
                moveTo(getX() - 1, getY());
                return;
            }
        case right:
            if (!getWorld()->isObstructed(getX() + 1, getY(), right)) {
                moveTo(getX() + 1, getY());
                return;
            }
    }
}
void ThiefBot::doSomething() {
    if (!isAlive())
        return;
    if (checkTick()) {
        setGoodie(getWorld()->goodieHere(getX(), getY()));
        if (getGoodie() != nullptr && randInt(1,10) == 1) {
            getGoodie()->setVisible(false);
            getWorld()->playSound(SOUND_ROBOT_MUNCH);
            return;
        }
        if (getDistanceBeforeTurning() > 0) {
            move();
        }
        else {
            setDistanceBeforeTurning(randInt(1, 6));
            vector<int> dir = {up, down, left, right};
            while (dir.size() != 0) {
                int pos = randInt(0, int(dir.size()) - 1);
                setDirection(dir.at(pos));
                if (!getWorld()->isObstructed(getX(), getY(), getDirection())) {
                    move();
                    return;
                }
                else
                    dir.erase(dir.begin() + pos);
            }
        }
    }
}

bool ThiefBot::getAttacked() {
    takeDamage(2);
    if (getHealth() <= 0) {
        setDead();
        getWorld()->playSound(SOUND_ROBOT_DIE);
        getWorld()->increaseScore(10);
        m_Goodie->setVisible(true);
        m_Goodie->moveTo(getX(), getY());
    }
    else {
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
    }
    return true;
}

void MeanThiefBot::doSomething() {
    if (!isAlive())
        return;
    if (checkTick()) {
        if (getWorld()->playerInSight(getX(), getY(), getDirection()))
            firePea();
        else {
            setGoodie(getWorld()->goodieHere(getX(), getY()));
            if (getGoodie() != nullptr && randInt(1,10) == 1) {
                getGoodie()->setVisible(false);
                getWorld()->playSound(SOUND_ROBOT_MUNCH);
                return;
            }
            if (getDistanceBeforeTurning() > 0) {
                move();
            }
            else {
                setDistanceBeforeTurning(randInt(1, 6));
                vector<int> dir = {up, down, left, right};
                while (dir.size() != 0) {
                    int pos = randInt(0, int(dir.size()) - 1);
                    setDirection(dir.at(pos));
                    if (!getWorld()->isObstructed(getX(), getY(), getDirection())) {
                        move();
                        return;
                    }
                    else
                        dir.erase(dir.begin() + pos);
                }
            }
        }
    }
}

bool MeanThiefBot::getAttacked() {
    takeDamage(2);
    if (getHealth() <= 0) {
        setDead();
        getWorld()->playSound(SOUND_ROBOT_DIE);
        getWorld()->increaseScore(20);
        getGoodie()->setVisible(true);
        getGoodie()->moveTo(getX(), getY());
    }
    else {
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
    }
    return true;
}

