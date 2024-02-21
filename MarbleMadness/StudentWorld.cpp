#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

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
}

int StudentWorld::init()
{
    // load level
    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel("level00.txt");
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
        for (int x = 0; x < VIEW_WIDTH; x++) {
            for (int y = 0; y < VIEW_HEIGHT; y++) {
                Level::MazeEntry item = lev.getContentsOf(x, y);
                switch(item) {
                    case Level::exit:
                        m_Actors.push_back(new Exit(x, y, this));
                        m_exitX = x;
                        m_exitY = y;
                        break;
                    case Level::player:
                        m_player = new Avatar(x, y, this);
                        break;
                    case Level::horiz_ragebot:
                        m_Actors.push_back(new RageBot(x, y, this, 90));
                        break;
                    case Level::vert_ragebot:
                        m_Actors.push_back(new RageBot(x, y, this, 270));
                        break;
                    case Level::thiefbot_factory:
                        m_Actors.push_back(new ThiefBotFactory(x, y, true, this));
                        break;
                    case Level::mean_thiefbot_factory:
                        m_Actors.push_back(new ThiefBotFactory(x, y, false, this));
                        break;
                    case Level::wall:
                        m_Actors.push_back(new Wall(x, y, this));
                        break;
                    case Level::marble:
                        m_Actors.push_back(new Marble(x, y, this));
                        break;
                    case Level::pit:
                        m_Actors.push_back(new Pit(x, y, this));
                        break;
                    case Level::crystal:
                        m_Actors.push_back(new Crystal(x, y, this));
                        m_crystals ++;
                        break;
                    case Level::restore_health:
                        m_Actors.push_back(new RestoreHealthGoodie(x, y, this));
                        break;
                    case Level::extra_life:
                        m_Actors.push_back(new ExtraLifeGoodie(x, y, this));
                        break;
                    case Level::ammo:
                        m_Actors.push_back(new AmmoGoodie(x, y, this));
                        break;
                    case Level::empty:
                        break;
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
//     This code is here merely to allow the game to build, run, and terminate after you type q
//    updateDisplayText();
    setGameStatText("Score: " + to_string(getScore()) + " Level: " + to_string(getLevel()) + " Lives: " + to_string(getLives()) + " Health: " + to_string(m_player->getHealth()) + " Ammo: " + to_string(m_player->getAmmo()) + " Bonus: " + to_string(m_bonus));
    
    // Make all actors do something, check if player dies after every action
    m_player->doSomething();
    for (size_t i = 0; i < m_Actors.size(); i ++) {
        if (m_Actors[i]->isAlive())
            m_Actors[i]->doSomething();
        if (!m_player->isAlive())
            return GWSTATUS_PLAYER_DIED;
    }
    
    // check if player has completed the level
    if (m_crystals == m_crystalsCollected && m_exitX == m_player->getX() && m_exitY == m_player->getY()) {
        increaseScore(2000 + m_bonus);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    // remove dead actors
    for (size_t i = 0; i < m_Actors.size(); i ++) {
        if (!m_Actors[i]->isAlive())
            delete m_Actors[i];
    }
    
    // reducing time bonus
    if (m_bonus > 0)
        m_bonus--;
    
    // revealing exit if all crystals are found
    if (m_crystals == m_crystalsCollected) {
        for (size_t i = 0; i < m_Actors.size(); i ++) {
            if (m_Actors[i]->isExit()) {
                m_Actors[i]->setVisible(IID_EXIT);
            }
        }
    }
	return GWSTATUS_CONTINUE_GAME;
    
}

void StudentWorld::addObject(Actor* object) {
    m_Actors.push_back(object);
}

bool StudentWorld::isObstructed(int x, int y, int direction) {
    bool empty = true;
    for (size_t i = 0; i < m_Actors.size(); i ++) {
        if (m_Actors[i]->getX() == x && m_Actors[i]->getY() == y) {
            if (m_Actors[i]->canPush(direction))
                ;
            else
                empty = false;
        }
    }
    return !empty;
}

bool StudentWorld::isPit(int x, int y) {
    for (size_t i = 0; i < m_Actors.size(); i ++) {
        if (m_Actors[i]->getX() == x && m_Actors[i]->getY() == y && m_Actors[i]->isPit())
            return true;
    }
    return false;
}

bool StudentWorld::isEmpty(int x, int y) {
    for (size_t i = 0; i < m_Actors.size(); i ++) {
        if (m_Actors[i]->getX() == x && m_Actors[i]->getY() == y)
            return false;
    }
    return true;
}

void StudentWorld::cleanUp()
{
    for (size_t i = 0; i != m_Actors.size(); i ++)
        delete m_Actors[i];
    delete m_player;
}
