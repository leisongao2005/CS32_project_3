#include "Actor.h"
#include "StudentWorld.h"

#include <iostream>
using namespace std;
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

bool Agent::moveIfPossible() {
    if (getWorld()->canAgentMoveTo(this, getX() + getdx(), getY() + getdy(), getdx(), getdy())) {
//        moveForward();
        moveTo(getX() + getdx(), getY() + getdy());
        return true;
    }
    return false;
}

void Player::doSomething() {
    cout << "player at " << getX() << ", " << getY() << endl;
    if (!isAlive())
        return;
    int ch;
    // must implement getWorld function
    if (getWorld()->getKey(ch)) {
        int dx = 0, dy = 0;
        switch (ch)
        {
            case KEY_PRESS_ESCAPE:
                setDead();
                break;
            case KEY_PRESS_SPACE: // fires a pea
                if (m_ammo > 0) {
                    getWorld()->addActor(new Pea(getWorld(), getX() + getdx(), getY() + getdy(), getDirection()));
                    m_ammo --;
                }
                break;
            case KEY_PRESS_UP:
                dy = 1;
                setDirection(up);
                break;
            case KEY_PRESS_DOWN:
                dy = -1;
                setDirection(down);
                break;
            case KEY_PRESS_LEFT:
                dx = -1;
                setDirection(left);
                break;
            case KEY_PRESS_RIGHT:
                dx = 1;
                setDirection(right);
                break;
        }
        if (dx != 0 || dy != 0) {
            moveIfPossible();
        }
    }
}

void Player::damage(int damageAmt) {
    if (getHitPoints() > damageAmt) {
        decHitPoints(damageAmt);
        getWorld()->playSound(SOUND_PLAYER_IMPACT);
    }
    else {
        setDead();
        getWorld()->playSound(SOUND_PLAYER_DIE);
    }
}

Robot::Robot(StudentWorld* world, int startX, int startY, int imageID,
      int hitPoints, int score, int startDir): Agent(world, startX, startY, imageID, hitPoints, startDir), m_score(score) {
    m_ticks = (28 - getWorld()->getLevel())/4;
    if (m_ticks < 3)
        m_ticks = 3;
    m_curr_tick = 0;
}

void Robot::damage(int damageAmt) {
    if (getHitPoints() > damageAmt) {
        decHitPoints(damageAmt);
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
    }
    else {
        setDead();
        getWorld()->playSound(SOUND_ROBOT_DIE);
        getWorld()->increaseScore(m_score);
        dieForSpecificRobotSubclass();
    }
}

void Robot::doSomething() {
    if (!isAlive())
        return;
    if (checkTick()) {
        if (isShootingRobot()) {
            // try to shoot
            if (getWorld()->existsClearShotToPlayer(getX(), getY(), getdx(), getdy())) {
                getWorld()->addActor(new Pea(getWorld(), getX() + getdx(), getY() + getdy(), getDirection()));
                return;
            }
        }
        // robot movement or picking up goodie
        doSomethingForSpecificRobotSubclass();
    }
}

void RageBot::doSomethingForSpecificRobotSubclass() {
    if (!moveIfPossible()) {
        setDirection(getDirection() + 180);
    }
}

void ThiefBot::doSomethingForSpecificRobotSubclass() {
    if (m_Goodie == nullptr && getWorld()->getColocatedStealable(getX(), getY()) != nullptr && randInt(1, 10) == 1) {
        m_Goodie = getWorld()->getColocatedStealable(getX(), getY());
        m_Goodie->setVisible(false);
        getWorld()->playSound(SOUND_ROBOT_MUNCH);
        return;
    }
    if (m_distanceBeforeTurning <= 0 || !moveIfPossible()) {
        m_distanceBeforeTurning = randInt(1, 6);
        vector<int> directions = {up, down, left, right};
        while (directions.size() != 0) {
            int dir = randInt(0, int(directions.size()) - 1);
            setDirection(directions.at(dir));
            if (moveIfPossible())
                return;
            else
                directions.erase(directions.begin() + dir);
        }
    }
}

void ThiefBot::dieForSpecificRobotSubclass() {
    if (m_Goodie != nullptr) {
        m_Goodie->setVisible(true);
        m_Goodie->moveTo(getX(), getY());
    }
}

void Exit::doSomething() {
    cout << "exit doing soemthing" << endl;
    cout << "exit at " << getX() << ", " << getY() << endl;
    if (isVisible())
        cout << "visible" << endl;
    if (isVisible() && getWorld()->isPlayerColocatedWith(getX(), getY())) {
        cout << "level done" << endl;
        getWorld()->playSound(SOUND_FINISHED_LEVEL);
        getWorld()->increaseScore(2000);
        getWorld()->setLevelFinished();
    }
}

bool Marble::bePushedBy(Agent* a, int x, int y) {
    if (getWorld()->canMarbleMoveTo(x, y) && getWorld()->isPlayerColocatedWith(getX() - a->getdx(), getY() - a->getdy())) {
        moveTo(x, y);
        return true;
    }
    return false;
}

void Pit::doSomething() {
    if (!isAlive())
        return;
    if (getWorld()->swallowSwallowable(this)) {
        setDead();
        // swallowSwallowable function kills marble
    }
}

void Pea::doSomething() {
    if (!isAlive())
        return;
    if (getWorld()->damageSomething(this, 2))
        setDead();
    else {
        moveTo(getX() + getdx(), getY() + getdy());
        if (getWorld()->damageSomething(this, 2))
            setDead();
    }
}

void ThiefBotFactory::doSomething() {
    int count = 0;
    if (getWorld()->doFactoryCensus(getX(), getY(), 3, count) && count < 3 && randInt(1, 50) == 1) {
        if (!m_meanBots)
            getWorld()->addActor(new RegularThiefBot(getWorld(), getX(), getY()));
        else
            getWorld()->addActor(new MeanThiefBot(getWorld(), getX(), getY()));
        getWorld()->playSound(SOUND_ROBOT_BORN);
    }
}

void PickupableItem::doSomething() {
    if(!isAlive())
        return;
    if (getWorld()->isPlayerColocatedWith(getX(), getY())) {
        getWorld()->increaseScore(m_score);
        doItemSpecificSomething();
        setDead();
    }
}

void Crystal::doItemSpecificSomething() {
    getWorld()->decCrystals();
    getWorld()->playSound(SOUND_GOT_GOODIE);
}

void Goodie::doItemSpecificSomething() {
    if (!isVisible())
        m_stolen = true;
    if (m_stolen)
        return;
    doGoodieSpecificSomething();
}

void ExtraLifeGoodie::doGoodieSpecificSomething() {
    getWorld()->incLives();
}

void RestoreHealthGoodie::doGoodieSpecificSomething() {
    getWorld()->restorePlayerHealth();
}

void AmmoGoodie::doGoodieSpecificSomething() {
    getWorld()->increaseAmmo();
}
