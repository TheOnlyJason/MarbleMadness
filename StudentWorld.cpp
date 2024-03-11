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
: GameWorld(assetPath), m_player(nullptr), status(GWSTATUS_CONTINUE_GAME)
{
    
}

void StudentWorld::addActor(Actor* actor) {
    actors.push_back(actor);
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    string curLevel = "level00.txt";
    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel(curLevel);
    status = GWSTATUS_CONTINUE_GAME;
    
    if (result == Level::load_fail_file_not_found ||
     result == Level:: load_fail_bad_format)
     return -1; // something bad happened!
    
    const int BOARD_WIDTH = VIEW_WIDTH;
    const int BOARD_HEIGHT = VIEW_HEIGHT;
    
    for (int y = 0; y < BOARD_HEIGHT; y++) {
            for (int x = 0; x < BOARD_WIDTH; x++) {
                Level::MazeEntry cellContents = lev.getContentsOf(x, y);
                
                switch (cellContents)
                {
                    case Level::MazeEntry::player:
                        m_player = new Player(this,x,y, 0);
                        actors.push_back(m_player);
                        break;
                    case Level::MazeEntry::wall:
                        actors.push_back(new Wall(this,x,y, 0));
                        break;
                    case Level::MazeEntry::pit:
                        actors.push_back(new Pit(this,x,y, 0));
                        break;
                    case Level::MazeEntry::marble:
                        actors.push_back(new Marble(this,x,y, 0));
                        break;
                    case Level::MazeEntry::crystal:
                        actors.push_back(new Crystal(this,x,y, 0));
                        break;
                    case Level::MazeEntry::extra_life:
                        actors.push_back(new ExtraLifeGoodie(this,x,y));
                        break;
                    case Level::MazeEntry::restore_health:
                        actors.push_back(new RestoreHealthGoodie(this,x,y));
                        break;
                    case Level::MazeEntry::ammo:
                        actors.push_back(new AmmoGoodie(this,x,y));
                        break;
                    default:
                        break;
                }
            }
        }
    
    return status;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you type q
    setGameStatText( "Score: " + std::to_string(getScore()) +
                    " Level: " + std::to_string(getLevel()) +
                    " Lives: " + std::to_string(getLives())
                    );
    
    std::vector<Actor*>::iterator it = actors.begin();
    while (it != actors.end()) {
        Actor* actor = *it;
        if (actor->isAlive()) {
            actor->doSomething();
            ++it;
        } else {
            delete actor;
            it = actors.erase(it);
        }
    }
    
	return GWSTATUS_CONTINUE_GAME;
}
bool StudentWorld::isEmpty(int x, int y)
{
    if (x < 0 || x >= VIEW_WIDTH || y < 0 || y >= VIEW_HEIGHT) {
            return false; // Position is out of bounds
        }

        // Check if there are any obstacles or other objects occupying the specified position
        for (const auto& actor : actors) {
            if (actor->getX() == x && actor->getY() == y) {
                return false; // Position is occupied by an object
            }
        }

        // If no objects are found at the specified position, it is considered empty
        return true;
}

void StudentWorld::cleanUp()
{

    m_player = nullptr;
    
    for (Actor* actor : actors) {
            delete actor;
        }
    actors.clear();
    
}


bool StudentWorld::isObstacle(int x, int y) const
{
    for (Actor* actor : actors) {
        if(actor->getX() == x && actor->getY() == y && !(actor->isMoveable()))
            return true;
    }
    return false;
}

void StudentWorld::gameOver()
{
    status = GWSTATUS_PLAYER_DIED;
}

Player* StudentWorld::getPlayer() const
{
    return m_player;
}



void StudentWorld::pushMarble(int playerX, int playerY, int playerDirection) {
    // Calculate the coordinates of the square in front of the player
    int nextX = playerX;
    int nextY = playerY;
    switch (playerDirection) {
        case GraphObject::up:
            nextY++;
            break;
        case GraphObject::down:
            nextY--;
            break;
        case GraphObject::left:
            nextX--;
            break;
        case GraphObject::right:
            nextX++;
            break;
    }
    
    // Check if the destination spot is empty or contains a pit
    if (isEmpty(nextX, nextY) /*|| getWorld()->isPit(nextX, nextY)*/) {
        // Iterate through the actors to find the marble at the current position
        for (Actor* actor : actors) {
            if (actor->isMarble() && actor->getX() == playerX && actor->getY() == playerY) {
                // Move the marble to the destination spot
                actor->moveTo(nextX, nextY);
                break; // Exit the loop after moving the marble
            }
        }
    }
}

