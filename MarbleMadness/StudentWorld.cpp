#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <format>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

string ZeroPadNumber(int num, int width);

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_crystalsCollected = 0;
    m_crystals = 0;
    m_bonus = 1000;
    m_level_finished = false;
}

int StudentWorld::init()
{
    // load level
    Level lev(assetPath());
//    Level::LoadResult result = lev.loadLevel("level0" + to_string(getLevel()) + ".txt");
    cout << to_string(getLevel()) << endl;
    Level::LoadResult result = lev.loadLevel("level01.txt");
    if (result == Level::load_fail_file_not_found) {
        cerr << "Could not find level00.txt data file\n";
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_fail_bad_format) {
        cerr << "Your level was improperly formatted\n";
        return GWSTATUS_LEVEL_ERROR;
    }
    
    // load succeeded
    else if (result == Level::load_success)
    {
        // iterating over level size and adding actors to actor container
        m_level_finished = false;
        for (int x = 0; x < VIEW_WIDTH; x++) {
            for (int y = 0; y < VIEW_HEIGHT; y++) {
                Level::MazeEntry item = lev.getContentsOf(x, y);
                switch(item) {
                    case Level::player:
                        m_player = new Player(this, x, y);
                        break;
                    case Level::wall:
                        m_Actors.push_back(new Wall(this, x, y));
                        break;
                    case Level::exit:
                        m_Actors.push_back(new Exit(this, x, y));
                        break;
                    case Level::horiz_ragebot:
                        m_Actors.push_back(new RageBot(this, x, y, 0));
                        break;
                   case Level::vert_ragebot:
                       m_Actors.push_back(new RageBot(this, x, y, 270));
                       break;
                   case Level::thiefbot_factory:
                       m_Actors.push_back(new ThiefBotFactory(this, x, y, false));
                       break;
                   case Level::mean_thiefbot_factory:
                       m_Actors.push_back(new ThiefBotFactory(this, x, y, true));
                       break;
                   case Level::marble:
                       m_Actors.push_back(new Marble(this, x, y));
                       break;
                   case Level::pit:
                       m_Actors.push_back(new Pit(this, x, y));
                       break;
                   case Level::crystal:
                       m_Actors.push_back(new Crystal(this, x, y));
                       m_crystals ++;
                       break;
                   case Level::restore_health:
                       m_Actors.push_back(new RestoreHealthGoodie(this, x, y));
                       break;
                   case Level::extra_life:
                       m_Actors.push_back(new ExtraLifeGoodie(this, x, y));
                       break;
                   case Level::ammo:
                       m_Actors.push_back(new AmmoGoodie(this, x, y));
                       break;
                   case Level::empty:
                       break;
                    default:
                        break;
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}
int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you type q
    
    setGameStatText("Score: " + ZeroPadNumber(getScore(), 7) + " Level: 0" + ZeroPadNumber(getLevel(), 2) + " Lives: " + to_string(getLives()) + " Health: " + to_string(m_player->getHealthPct()) + " Ammo: " + to_string(m_player->getAmmo()) + " Bonus: " + to_string(m_bonus));
    
    // Make all actors do something, check if player dies after every action
    m_player->doSomething();
    for (size_t i = 0; i < m_Actors.size(); i++) {
        if (m_Actors[i]->isAlive())
            m_Actors[i]->doSomething();
        if (!m_player->isAlive())
            return GWSTATUS_PLAYER_DIED;
    }
    
    // check if player has completed the level
    if (m_level_finished) {
        increaseScore(m_bonus);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    // remove dead actors
    for (size_t i = 0; i < m_Actors.size();) {
        if (!m_Actors[i]->isAlive()) {
            delete m_Actors[i];
            m_Actors.erase(m_Actors.begin() + i);
        }
        else
            i ++;
    }
    
    // reducing time bonus
    if (m_bonus > 0)
        m_bonus--;
    
    // revealing exit if all crystals are found
    if (!anyCrystals()) {
        for (size_t i = 0; i < m_Actors.size(); i ++) {
            if (m_Actors[i]->isExit() && !m_Actors[i]->isVisible()) {
                m_Actors[i]->setVisible(IID_EXIT);
                playSound(SOUND_REVEAL_EXIT);
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
        for (size_t i = 0; i != m_Actors.size();) {
            delete m_Actors[i];
            m_Actors.erase(m_Actors.begin() + i);
        }
        delete m_player;
        m_crystals = 0;
        m_crystalsCollected = 0;
}

// Can agent move to x,y?  (dx and dy indicate the direction of motion)
bool StudentWorld::canAgentMoveTo(Agent* agent, int x, int y, int dx, int dy) const {
    for (size_t i = 0; i != m_Actors.size(); i ++) {
        Actor* a = m_Actors.at(i);
        if (a->getX() == x && a->getY() == y) {
            if (a->allowsAgentColocation() || a->bePushedBy(agent, x + dx, y + dy))
                return true;
            else
                return false;
        }
    }
    return true;
}

// Can a marble move to x,y?
bool StudentWorld::canMarbleMoveTo(int x, int y) const {
    for (size_t i = 0; i != m_Actors.size(); i ++) {
        Actor* a = m_Actors.at(i);
        if (a->getX() == x && a->getY() == y && !a->allowsMarble()) {
            return false;
        }
    }
    return true;
}

// Is the player on the same square as an Actor?
bool StudentWorld::isPlayerColocatedWith(int x, int y) const {
    if (m_player->getY() == y)
        cout << "yes" << endl;
    return m_player->getX() == x && m_player->getY() == y;
}


// Try to cause damage to something at a's location.  (a is only ever
// going to be a pea.)  Return true if something stops a -- something
// at this location prevents a pea from continuing.
bool StudentWorld::damageSomething(Actor* a, int damageAmt) {
    int x = a->getX();
    int y = a->getY();
    bool damaged = false;
    if (m_player->getX() == x && m_player->getY() == y) {
        m_player->damage(damageAmt);
        damaged = true;
    }
    for (size_t i = 0; i != m_Actors.size(); i ++) {
        Actor* object = m_Actors.at(i);
        if (object->getX() == x && object->getY() == y) {
            if (object->isDamageable()) {
                object->damage(damageAmt);
                damaged = true;
            }
            if (object->stopsPea())
                damaged = true;
        }
    }
    return damaged;
}

// Swallow any swallowable object at a's location.  (a is only ever
// going to be a pit.)
bool StudentWorld::swallowSwallowable(Actor* a) {
    int x = a->getX();
    int y = a->getY();
    for (size_t i = 0; i != m_Actors.size(); i ++) {
        Actor* object = m_Actors.at(i);
        if (object->getX() == x && object->getY() == y && object->isSwallowable()) {
            object->setDead();
            return true;
        }
    }
    return false;
}


// If a pea were at x,y moving in direction dx,dy, could it hit the
// player without encountering any obstructions?
bool StudentWorld::existsClearShotToPlayer(int x, int y, int dx, int dy) const {
    int px = m_player->getX();
    int py = m_player->getY();
    while (true) {
        x += dx;
        y += dy;
        if (px == x && py == y)
            return true;
        else {
            for (size_t i = 0; i != m_Actors.size(); i ++) {
                Actor* a = m_Actors.at(i);
                if (a->getX() == x && a->getY() == y && a->stopsPea()) {
                    return false;
                }
            }
        }
    }
    return false;
}

// If a factory is at x,y, how many items of the type that should be
// counted are in the rectangle bounded by x-distance,y-distance and
// x+distance,y+distance?  Set count to that number and return true,
// unless an item is on the factory itself, in which case return false
// and don't care about count.  (The items counted are only ever going
// ThiefBots.)
bool StudentWorld::doFactoryCensus(int x, int y, int distance, int& count) const {
    count = 0;
    for (size_t i = 0; i != m_Actors.size(); i++) {
        Actor* a = m_Actors.at(i);
        if (a->countsInFactoryCensus()) {
            if (a->getX() == x && a->getY() == y) {
                return false;
            }
            else if(a->getX() >= x - 3 && a->getX() <= x + 3 &&
                    a->getY() >= y - 3 && a->getY() <= y + 3) {
                count ++;
            }
        }
    }
    return true;
}

// If an item that can be stolen is at x,y, return a pointer to it;
// otherwise, return a null pointer.  (Stealable items are only ever
// going be goodies.)
Actor* StudentWorld::getColocatedStealable(int x, int y) const {
    for (size_t i = 0; i != m_Actors.size(); i++) {
        Actor* a = m_Actors.at(i);
        if (a->getX() == x && a->getY() == y && a->isStealable() && a->isVisible())
            return a;
    }
    return nullptr;
}

void StudentWorld::restorePlayerHealth() {
    m_player->restoreHealth();
}

void StudentWorld::increaseAmmo() {
    m_player->increaseAmmo();
}

string ZeroPadNumber(int num, int width)
{
    ostringstream ss;
    ss << setw( width ) << setfill( '0' ) << num;
    return ss.str();
}

