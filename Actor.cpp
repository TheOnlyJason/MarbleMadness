#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageIDS, StudentWorld* sw, double x, double y, bool canMove, int startDir): GraphObject(imageIDS, x, y, startDir), m_world(sw) , moveable(canMove), alive(true)
{
    
    setDirection(0);
}

bool Actor::isMarble() const
{
    return false;
}

StudentWorld* Actor::getWorld() const {
    return m_world;
}
void  Actor::setDead()
{
    alive = false;
}
bool Actor::isAlive() const{
    return alive;
}

bool Actor::isMoveable()
{
    return moveable;
}

bool Actor::isDamageable() const
{
    return true;
}


bool Actor::isSwallowable() const
{
    return false;
}
// wall
Wall::Wall(StudentWorld* sw, int x, int y, int startDir) : Actor(IID_WALL, sw, x, y, false, startDir)
{
    setVisible(true);
}


void Wall::doSomething()
{
    return;
}

// agent
Agent::Agent(int  imageID, StudentWorld* world, int startX, int startY, int hitPoints, int startDir) : Actor(imageID, world, startX, startY, true, startDir), m_hitPoints(hitPoints)
{

}



int Agent::shootingSound() const
{
    return SOUND_PLAYER_FIRE;
}

void Agent::doSomething()
{
    
}

bool Agent::canPushMarbles() const
{
    return false;
}

bool Agent::needsClearShot() const
{
    return true;
}

void Agent::setHitPoint(int amt)
{
    m_hitPoints = amt;
}

bool Agent::isDamageable() const
{
    return true;
}




// player
Player::Player(StudentWorld* world, double startX, double startY, int  startDir): Agent(IID_PLAYER, world, startX, startY, 20, 0), m_peas(20)
{
    setVisible(true);
}

bool Player::canPushMarbles() const {
    // Get the coordinates of the space in front of the player
    int nextX = getX();
    int nextY = getY();
    switch (getDirection()) {
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
    // Check if the space in front of the player is empty
    return getWorld()->isEmpty(nextX, nextY);
}

bool Player::needsClearShot() const
{
    StudentWorld* w = getWorld();
    int key;
    w->getKey(key);
    
    Player* p = w->getPlayer();
    int dir = p->getDirection();
    // need to make sure that this does count the walls
    if(dir == left)
    {
        for (int i = p->getX(); i > 0; i--) {
            if(w->isObstacle(getX() - i, getY())){
                return false;
            }
        }
    }
    else if(dir == 90)
    {
        for (int i = p->getX(); i < VIEW_WIDTH; i++) {
            if(w->isObstacle(getX() + i, getY())){
                return false;
            }
        }
    }
    else if( dir == right)
    {
        for (int i = p->getX(); i < VIEW_WIDTH; i++) {
            if(w->isObstacle(getX() + i, getY())){
                return false;
            }
        }
    }
    else if (dir == -90){
        for (int i = p->getX(); i < VIEW_WIDTH; i++) {
            if(w->isObstacle(getX() + i, getY())){
                return false;
            }
        }
    }
    return true;
}

void Player::shootpeas()
{
    if (m_peas > 0) { // Check if the player has peas available
        // Get the current coordinates and direction of the player
        int x = getX();
        int y = getY();
        int dir = getDirection();

        // Calculate the coordinates of the square immediately in front of the player
        
        switch (dir) {
            case GraphObject::up:
                y++;
                break;
            case GraphObject::down:
                y--;
                break;
            case GraphObject::left:
                x--;
                break;
            case GraphObject::right:
                x++;
                break;
        }
        // Create a new Pea object at the calculated coordinates and direction
        getWorld()->addActor(new Pea(getWorld(), x, y, dir));

        // Play the SOUND_PLAYER_FIRE sound effect
        getWorld()->playSound(SOUND_PLAYER_FIRE);

        m_peas--;
    }
}


int Player::shootingSound() const
{
    return SOUND_PLAYER_FIRE;
}


int Player::getAmmo() const
{
    return m_peas;
}

void Player::restoreHealth()
{
    setHitPoint(20);
}
    
// Increase player's amount of ammunition.
void Player::increaseAmmo()
{
    m_peas += 20;
}

void Player::doSomething()
{
    StudentWorld* w = getWorld();
    
    int key;
    if (w->getKey(key)) {
        switch (key) {
            case KEY_PRESS_ESCAPE:
                break;
            case KEY_PRESS_UP:
                setDirection(90);
                if(!(w->isObstacle(getX(), getY() + 1))){
                    getWorld()->pushMarble(getX(), getY() + 1, 90);
                    moveTo(getX(), getY() + 1);
                }
                break;
            case KEY_PRESS_DOWN:
                setDirection(-90);
                if(!(w->isObstacle(getX(), getY() - 1)))
                {
                    getWorld()->pushMarble(getX(), getY() -1, -90);
                    moveTo(getX(), getY() - 1);
                    
                }
                break;
            case KEY_PRESS_LEFT:
                setDirection(left);
                if(!(w->isObstacle(getX() - 1, getY())))
                {
                    getWorld()->pushMarble(getX() - 1, getY(), left);
                    
                    moveTo(getX() - 1, getY());
                }
                    
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                if(!(w->isObstacle(getX() + 1, getY())))
                {
                    getWorld()->pushMarble(getX() + 1, getY(), right);
                    moveTo(getX() + 1, getY());
                }
                    
                break;
            case KEY_PRESS_SPACE:
                shootpeas();
                break;
            default:
                break;
        }
    }
}
bool Player::isDamageable() const
{
    return true;
}


// pit
Pit::Pit(StudentWorld* world, double startX, double startY, int startDir): Actor(IID_PIT, world, startX, startY, false, startDir)
{
    setVisible(true);
}

void Pit::doSomething()
{
    
}



// marble
Marble::Marble(StudentWorld* world, double startX, double startY , int startDir) : Actor(IID_MARBLE, world, startX, startY, false, startDir)
{
    setVisible(true);
}

void Marble::doSomething()
{
    
    
}

bool Marble::isMarble() const
{
    return true;
}

bool Marble::isDamageable() const
{
    return true;
}

bool Marble::isSwallowable() const
{
    return true;
}

// pea
Pea::Pea(StudentWorld* world, double startX, double startY, int startDir):Actor(IID_PEA, world, startX, startY, true, startDir)
{
    setDirection(startDir);
    setVisible(true);
}
//pea shot is werid

//moving the merble is werid

// need to do the goodies and robot interaction

// need to do the bots

void Pea::doSomething()
{
    StudentWorld* w = getWorld();
    int dir = getDirection();

    int x = getX();
    int y = getY();
    
    switch (dir) {
        case GraphObject::up:
            if(!(w->isObstacle(getX(), getY() + 1)))
                moveTo(x, y + 1);
            else
                setDead();
            break;
        case GraphObject::down:
            if(!(w->isObstacle(getX(), getY() - 1)))
                moveTo(x, y - 1);
            else
                setDead();
            break;
        case GraphObject::left:
            if(!(w->isObstacle(getX() - 1 , getY())))
                moveTo(x - 1, y);
            else
                setDead();
            break;
        case GraphObject::right:
            if(!(w->isObstacle(getX() + 1,  getY())))
                moveTo(x + 1, y);
            else
                setDead();
            break;
        }
}


//exit
Exit::Exit(StudentWorld* world, double startX, double startY, int startDir): Actor(IID_EXIT, world, startX, startY, false, startDir)
{
    setVisible(false);
}

void Exit::doSomething()
{
    
}

PickupableItem::PickupableItem(StudentWorld* world, int startX, int startY, int imageID, int score) :Actor(imageID, world, startX, startY, true, 0), m_points(score)
{
    
}

void PickupableItem::doSomething()
{
    if(!isAlive())
    {
        return;
    }
    StudentWorld* w = getWorld();
    
    Player* p = w->getPlayer();
    
    if((p->getX() == getX()) && (p->getY() == getY()))
    {
        w->increaseScore(getPoints());
        setDead();
    }
}

int PickupableItem::getPoints() const
{
    return m_points;
}

//crystal
Crystal::Crystal(StudentWorld* world, double startX, double startY, int startDir): PickupableItem(world, startX, startY, IID_CRYSTAL, 50)
{
    setVisible(true);
}



Goodie::Goodie(StudentWorld* world, int startX, int startY, int imageID, int score) : PickupableItem(world, startX, startY, imageID, score), stolenYet(false)
{
    setVisible(true);
}

void Goodie::doSomething()
{
    if(!isAlive())
    {
        return;
    }
    StudentWorld* w = getWorld();
    
    Player* p = w->getPlayer();
    
    if((p->getX() == getX()) && (p->getY() == getY()))
    {
        w->increaseScore(getPoints());
        setDead();
    }
}

bool Goodie::isStealable() const
{
    return true;
}

void Goodie::setStolen(bool status)
{
    stolenYet = true;
}

ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* world, int startX, int startY) : Goodie(world, startX, startY, IID_EXTRA_LIFE, 1000)
{
        setVisible(true);

}

RestoreHealthGoodie::RestoreHealthGoodie(StudentWorld* world, int startX, int startY) : Goodie(world, startX, startY, IID_RESTORE_HEALTH, 500)
{
    setVisible(true);
}


AmmoGoodie::AmmoGoodie(StudentWorld* world, int startX, int startY):Goodie(world, startX, startY, IID_AMMO, 100)
{
    setVisible(true);
}

Robot::Robot(StudentWorld* world, int startX, int startY, int imageID,
int hitPoints, int score, int startDir) : Agent(imageID, world, startX, startY, hitPoints, 0)
{
    
}

void Robot::doSomething() const
{
    
}

bool  Robot::isDamageable() const
{
    return true;
}


// RageBot
RageBot::RageBot(StudentWorld* world, int startX, int startY, int startDir) :Robot(world, startX, startY, IID_RAGEBOT, 20, 100, 0)
{
    
}

void RageBot::doSomething()
{
    
}


ThiefBot::ThiefBot(StudentWorld* world, int startX, int startY, int imageID, int hitPoints, int score) : Robot(world, startX, startY, imageID, 20, 100, 0)
{
    
}

void ThiefBot::doSomething()
{
    
}

bool ThiefBot::countsInFactoryCensus() const
{
    return false;
}

void ThiefBot::damage(int damageAmt)
{
    
}

RegularThiefBot::RegularThiefBot(StudentWorld* world, int startX, int startY): ThiefBot(world, startX, startY, IID_THIEFBOT, 20, 100)
{
    
}

void RegularThiefBot::doSomething()
{
    
}

bool RegularThiefBot::isShootingRobot() const
{
    return true;
}


MeanThiefBot::MeanThiefBot(StudentWorld* world, int startX, int startY): ThiefBot(world, startX, startY, IID_MEAN_THIEFBOT, 20, 100)
{
    
}

void MeanThiefBot::doSomething()
{
    
}

