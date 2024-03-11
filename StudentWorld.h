#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <vector>
#include <string>
#include "Actor.h"
//#include "Wall.h"
//#include "Pit.h"
//#include "Marble.h"
//#include "Player.h"

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Player;

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  ~StudentWorld();
  virtual int init();
  virtual int move();
  virtual void cleanUp();
    
  void gameOver();
    
  bool isEmpty(int x, int y);
    
  bool isObstacle(int x, int y) const;
    
  void addActor(Actor* actor);
    
  void pushMarble (int playerX, int playerY, int playerDirection);
    


  
  Player* getPlayer() const;

private:
    
    Player* m_player;
    std::vector<Actor*> actors;
    int status;

};

#endif // STUDENTWORLD_H_
