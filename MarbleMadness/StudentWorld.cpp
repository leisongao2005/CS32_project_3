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
                        m_Actors.push_back(new Exit(x, y));
                        m_exitX = x;
                        m_exitY = y;
                        break;
                    case Level::player:
                        m_player = new Avatar(x, y);
                        break;
                    case Level::horiz_ragebot:
                        m_Actors.push_back(new RageBot(x, y, 90));
                        break;
                    case Level::vert_ragebot:
                        m_Actors.push_back(new RageBot(x, y, 270));
                        break;
                    case Level::thiefbot_factory:
                        m_Actors.push_back(new ThiefBotFactory(x, y, true));
                        break;
                    case Level::mean_thiefbot_factory:
                        m_Actors.push_back(new ThiefBotFactory(x, y, false));
                        break;
                    case Level::wall:
                        m_Actors.push_back(new Wall(x, y));
                        break;
                    case Level::marble:
                        m_Actors.push_back(new Marble(x, y));
                        break;
                    case Level::pit:
                        m_Actors.push_back(new Pit(x, y));
                        break;
                    case Level::crystal:
                        m_Actors.push_back(new Crystal(x, y));
                        m_crystals ++;
                        break;
                    case Level::restore_health:
                        m_Actors.push_back(new RestoreHealthGoodie(x, y));
                        break;
                    case Level::extra_life:
                        m_Actors.push_back(new ExtraLifeGoodie(x, y));
                        break;
                    case Level::ammo:
                        m_Actors.push_back(new AmmoGoodie(x, y));
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
    for (size_t i = 0; i < m_Actors.size(); i ++) {
        m_Actors[i]->doSomething();
        if (!m_player->isAlive())
            return GWSTATUS_PLAYER_DIED;
        if (m_crystals == m_crystalsCollected && m_exitX == m_player->getX() && m_exitY == m_player->getY()) {
            increaseScore(2000 + m_bonus);
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    // must delete actors that have died
    
    if (m_bonus > 0)
        m_bonus--;
    setGameStatText("Game will end when you type q");
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
}
