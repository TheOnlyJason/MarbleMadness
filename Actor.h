#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


class Actor: public GraphObject
{
public:
    Actor(int imageIDS, StudentWorld* sw, double x, double y, bool canMove, int startDir);
    
    StudentWorld* getWorld() const;
    
    virtual void doSomething() = 0;
    
    bool isMoveable();
    
    // Is this actor alive?
    bool isAlive() const;
  
    // Mark this actor as dead
    void setDead();
  
    // Reduce this actor's hit points
    void decHitPoints(int amt);
    
    virtual bool isMarble() const;
    
    virtual bool isDamageable() const;
    
    virtual bool isSwallowable() const;

    

    
private:
    StudentWorld* m_world;
    bool moveable;
    bool alive;
};

class Agent : public Actor
{
public:
    Agent(int imageID, StudentWorld* world, int startX, int startY,
                int hitPoints, int startDir);

    virtual void doSomething();
      // Move to the adjacent square in the direction the agent is facing
    
    virtual bool canPushMarbles() const;

    // Return true if this agent doesn't shoot unless there's an unobstructed
    // path to the player.
    virtual bool needsClearShot() const;
    
    // Return the sound effect ID for a shot from this agent.
    virtual int shootingSound() const;
    
    virtual bool isDamageable() const;
    
    
    void setHitPoint(int amt);
    
private:
    int m_hitPoints;
    
};

class Player : public Agent
{
public:
    Player(StudentWorld* world, double startX, double startY, int startDir);
    void doSomething();
    
    virtual bool canPushMarbles() const;
    
    virtual bool needsClearShot() const;
    
    virtual int shootingSound() const;
    
    virtual bool isDamageable() const;

    
    void shootpeas();
    
    int getAmmo() const;
    
    void restoreHealth();
        
    // Increase player's amount of ammunition.
    void increaseAmmo();
    
    
private:
    int m_peas;
    
    
};

class Wall : public Actor
{
public:
    Wall(StudentWorld* sw, int startX, int startY, int startDir);
    
    void doSomething();
    
    
private:
    
    
};

class Pit : public Actor
{
public:
    Pit(StudentWorld* world, double startX, double startY,int startDir);
    
    void doSomething();
    
private:
};



class Marble : public Actor
{
public:
    Marble(StudentWorld* world, double startX, double startY, int startDir);
    
    void doSomething();
    
    virtual bool isDamageable() const;
    
    virtual bool isSwallowable() const;


    
    virtual bool isMarble() const;
    
private:
};

class Pea : public Actor
{
public:
    Pea(StudentWorld* world, double startX, double startY, int startDir);
    
    void doSomething();
    
private:
};



class Exit : public Actor
{
public:
    Exit(StudentWorld* world, double startX, double startY, int startDir);
    
    void doSomething();
    
private:
};

class PickupableItem : public Actor
{
public:
    PickupableItem(StudentWorld* world, int startX, int startY, int imageID,
                            int score);
    virtual void doSomething();
    
    int getPoints() const;
    
private:
    int m_points;
    
};

class Crystal : public PickupableItem
{
public:
    Crystal(StudentWorld* world, double startX, double startY, int startDir);
    
    
private:
};


class Goodie : public PickupableItem
{
public:
    Goodie(StudentWorld* world, int startX, int startY, int imageID,
                            int score);
    virtual void doSomething();
    virtual bool isStealable() const;

      // Set whether this goodie is currently stolen.
    void setStolen(bool status);
    
private:
    bool stolenYet;
};


class ExtraLifeGoodie : public Goodie
{
public:
    ExtraLifeGoodie(StudentWorld* world, int startX, int startY);
};

class RestoreHealthGoodie : public Goodie
{
public:
    RestoreHealthGoodie(StudentWorld* world, int startX, int startY);
};

class AmmoGoodie : public Goodie
{
public:
    AmmoGoodie(StudentWorld* world, int startX, int startY);
};

class Robot : public Agent
{
public:
    Robot(StudentWorld* world, int startX, int startY, int imageID,
        int hitPoints, int score, int startDir);
    virtual void doSomething() const;
    virtual bool isDamageable() const;



      // Does this robot shoot?
};

class RageBot : public Robot
{
public:
    RageBot(StudentWorld* world, int startX, int startY, int startDir);
    virtual void doSomething();
};


class ThiefBot : public Robot
{
public:
    ThiefBot(StudentWorld* world, int startX, int startY, int imageID,
                         int hitPoints, int score);
    virtual void doSomething();
    virtual bool countsInFactoryCensus() const;
    virtual void damage(int damageAmt);
};

class RegularThiefBot : public ThiefBot
{
public:
    RegularThiefBot(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual bool isShootingRobot() const;
};

class MeanThiefBot : public ThiefBot
{
public:
    MeanThiefBot(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
};


#endif // ACTOR_H_
