#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <algorithm>
using namespace std;

#include <iostream>
#include <cmath>
#include <sstream>
#include "Actor.h"
#include <list>

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    m_socrates = nullptr;
    m_numenemieskilled = 0;
    m_targetkillcount = 10 * (getLevel());
    
    //add pits
    for (int i = 0; i < getLevel(); i++)
    {
        int x = randInt(0, VIEW_WIDTH-1);
        int y = randInt(0, VIEW_HEIGHT-1);
        
        if (initoverlap(x, y) || !isinbound(x, y, 1) )
        {
            i--;
        }
        else {
            Pit* newpit = new Pit(this, x, y);
            m_actors.push_back(newpit);
        }
    }
        
    
    //add food
    for (int i = 0; i < min(5*getLevel(), 25); i++)
    {
        //Food(StudentWorld* studentworld, double startX, double startY)
        int x = randInt(0, VIEW_WIDTH-1);
        int y = randInt(0, VIEW_HEIGHT-1);

        if (initoverlap(x, y) || !isinbound(x, y, 1) )
        {
            i--;
        }
        else {
            Food* newfood = new Food(this, x, y);
            m_actors.push_back(newfood);
        }
    }

    //add dirtpiles
    for (int i = 0; i < max(180-20*getLevel(), 20); i++)
    {
        //Dirt(StudentWorld* studentworld, double startX, double startY)
        int x = randInt(0, VIEW_WIDTH-1);
        int y = randInt(0, VIEW_HEIGHT-1);

        if (initoverlap(x, y) || !isinbound(x, y, 1) )
        {
            i--;
        }
        else {
            Dirt* newdirt = new Dirt(this, x, y);
            m_actors.push_back(newdirt);
        }

    }
    
    m_socrates = new Socrates(this);
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{

    if (m_socrates->isalive())
        m_socrates->doSomething();
    
    list<Actor*>::iterator it = m_actors.begin();


    while (it != m_actors.end())
    {
        // at every tick, check to see if the actors are alive, make them do something
        if ((*it)->isalive())
        {
            (*it)->doSomething();

//            if socrates died as a result, end the level
            if (!m_socrates->isalive())
            {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            if (socrateswon())
            {
                playSound(SOUND_FINISHED_LEVEL);
                return GWSTATUS_FINISHED_LEVEL;
            }
        }
        it++;
    }
    
    
    
    removedeadobjects();
    
    addnewactors();
    
    updatedisplaytext();
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    if (m_socrates != nullptr)
    {
        delete m_socrates;
        m_socrates = nullptr;
    }
    list<Actor*>::iterator it = m_actors.begin();
    while (it != m_actors.end())
    {
        delete *it;
        m_actors.erase(it);
        it++;
    }
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}


Socrates* StudentWorld::getSocrates() const
{
    return m_socrates;
}

void StudentWorld::addactor(Actor* a)
{
    m_actors.push_back(a);
}

//bool StudentWorld::iscollidingSocrates(const Actor* a) const
//{
//    if (isWithinEuclidianDistance(a, getSocrates()))
//    {
//        return true;
//    }
//    return false;
//}
//


bool StudentWorld::iscollidingdirt(double x, double y) const
{
    list<Actor*>::const_iterator it;

    for (it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if (isoverlapping(x, y, *it) && (*it)->isdirt())
        {
            return true;
        }
    }
    return false;
}



bool StudentWorld::iscollidingActor(double x, double y) const
{
    list<Actor*>::const_iterator it;

    for (it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if (isWithinEuclidianDistance(x, y, *it) && !(*it)->isdirt())
            return true;
    }
    
//    if (getSocrates() != nullptr && isWithinEuclidianDistance(x, y, getSocrates()))
//        return getSocrates();
    
    return false;
}

bool StudentWorld::iscollidingwithfood(double x, double y)
{
    list<Actor*>::iterator food;

    for (food = m_actors.begin(); food != m_actors.end(); food++)
    {
        if (isWithinEuclidianDistance(x, y, *food) && (*food)->isedible() && (*food)->isenemy())
        {
            
            (*food)->die();
            return true;
        }
    }
    return false;
}

bool StudentWorld::iscollidingprojectile(double x, double y, int losehealth)
{
    //iterates through actor list.
    list<Actor*>::iterator it;

    for (it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if (isWithinEuclidianDistance(x, y, *it) && (*it)->isdamageable() && (*it)->isalive())
        {
            if ((*it)->isdirt() || (*it)->isedible()) //takes into account dirt and goodies
            {
                (*it)->die();
            }
            
            else
            {
                 (*it)->losehp(losehealth); //takes into account bacteria
            }
             
            return true;
        }
    }
    return false;
}

//there are two types of inbound, one inbounds check occurs during init, <120, and one check occurs at the wall, <= 128. bacterias' movements will be blocked if they attempt to leave the circle.
bool StudentWorld::isinbound(double x, double y, int type) const
{
    if (type == 0) //type 0 is the wall
    {
        return (sqrt(pow(x - VIEW_WIDTH/2, 2) + pow(y - VIEW_HEIGHT/2, 2)) <= 128 );
    }
    
    else if (type == 1) // type 1 is dirt init
    {
        return (sqrt(pow(x - VIEW_WIDTH/2, 2) + pow(y - VIEW_HEIGHT/2, 2)) < 120);
    }
    
    return false;
}

//salmonella will check if food is nearby
bool StudentWorld::foodisnearby(Actor *a)
{
    list<Actor*>::const_iterator it;
    
    Actor* closestfood = nullptr;
    int closestdistance = 10000;
    
    for (it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if((*it)->isedible() && (*it)->isenemy())
        {
            double f_x = (*it)->getX();
            double f_y = (*it)->getY();
            double dx = f_x - a->getX();
            double dy = f_y - a->getY();
            double distance = sqrt(pow(dx, 2) + pow(dy, 2));
            if (distance < 128 && distance < closestdistance)
            {
                closestfood = *it;
                closestdistance = distance;
            }
        }
    }
//    return closestfood;
    if (closestfood != nullptr)
    {
        //sets direction to that closestfood. "target"
        a->setDirection(setdirectiontotarget(closestfood, a));
        return true;
    }
    return false;
}



bool StudentWorld::socratesisnearby(Actor *a, int range)
{
//very similar functionality as foodisnearby, except this applies to socrates.
    double f_x = getSocrates()->getX();
    double f_y = getSocrates()->getY();
    double dx = f_x - a->getX();
    double dy = f_y - a->getY();
    if (sqrt(pow(dx, 2) + pow(dy, 2)) < range )
    {
        a->setDirection(setdirectiontotarget(getSocrates(), a));
        return true;
    }
        
    return false;
}
                
int StudentWorld::setdirectiontotarget(const Actor* target, const Actor* b) const
{
    double x_target = target->getX();
    double y_target = target->getY();
     
    double dx = x_target - b->getX();
    double dy = y_target - b->getY();
     
    double new_dir = 180/(3.14159265)* atan2(dy ,dx);
    if (new_dir < 0)
        return new_dir + 360;
    return new_dir;
}

void StudentWorld::moveAroundCircle(double angle, double& new_x, double& new_y)
{
    new_x = VIEW_RADIUS+VIEW_RADIUS*cos(angle*(4*atan(1)/180));
    new_y = VIEW_RADIUS+VIEW_RADIUS*sin(angle*(4*atan(1)/180));

}

void StudentWorld::decpits()
{
    m_pits--;
}

void StudentWorld::increaseEnemiesKilled()
{
    m_numenemieskilled++;
}

void StudentWorld::increaseTargetkillcount()
{
    m_targetkillcount++;
}

///////////////////////////////////////////////////////////////////////////////
/////////PRIVATE FUNCTIONS/////
///////////////////////////////////////////////////////////////////////////////

bool StudentWorld::socrateswon()
{
    return (m_numenemieskilled == m_targetkillcount);
}

void StudentWorld::removedeadobjects()
{
    
    if (m_socrates != nullptr && ! m_socrates->isalive())
    {
        delete m_socrates;
        m_socrates = nullptr;
    }
    
    list<Actor*>::iterator it = m_actors.begin();
    
    while (it != m_actors.end())
    {
        if (!(*it)->isalive())
        {
            delete *it;
            it = m_actors.erase(it);
        }
        else
            it++;
    }
}

void StudentWorld::addnewactors()
{
    int chancefungus = max(510 - getLevel()*10, 200);
    int chancegoodie = max(510 - getLevel()*10, 250);

    if (randInt(0, chancefungus) == 0)
    {
        double x, y;
        moveAroundCircle(randInt(0, 359), x, y);
        Fungus* newfungus = new Fungus(this, x, y);
        m_actors.push_back(newfungus);
    }
    
    if (randInt(0, chancegoodie) == 0)
    {
        int goodietype = randInt(0, 10);
        if (goodietype <= 6)
        {
            double x, y;
            moveAroundCircle(randInt(0, 359), x, y);
            RestorehealthGoods* newrestorehealthgoodie = new RestorehealthGoods(this, x, y);
            m_actors.push_back(newrestorehealthgoodie);
        }
        if (goodietype > 6 && goodietype <= 9)
        {
            double x, y;
            moveAroundCircle(randInt(0, 359), x, y);
            FlameThrowerGoods* newflamethrowergoodie = new FlameThrowerGoods(this, x, y);
            m_actors.push_back(newflamethrowergoodie);
        }
        if (goodietype == 10)
        {
            double x, y;
            moveAroundCircle(randInt(0, 359), x, y);
            ExtraLifeGoods* newextralifegoodie = new ExtraLifeGoods(this, x, y);
            m_actors.push_back(newextralifegoodie);
        }
    }
}


void StudentWorld::updatedisplaytext()
{
    //if socrates is dead, return. dont update text
    if (getSocrates() == nullptr)
        return;
    
    ostringstream oss;
    
    oss << "Score: ";
    oss << getScore() << "  ";
    
    oss << "Level: ";
    oss << getLevel() << "  ";
    
    oss << "Lives: ";
    oss << getLives() << "  ";
    
    oss << "health: ";
    oss << getSocrates()->hp() << " ";
    
    oss << "Sprays: ";
    oss << getSocrates()->spraycharges() << " ";
    
    oss << "Flames: ";
    oss << getSocrates()->flamecharges();
    
    string result = oss.str();
    setGameStatText(result);
    
}

bool StudentWorld::isWithinEuclidianDistance(double x, double y, const Actor* b) const
{
    double dx = x - b->getX();
    double dy = y - b->getY();
    double distance = sqrt(dx*dx + dy*dy);
    
    if (distance <= SPRITE_RADIUS*2)
    {
        return true;
    }
    return false;
}


bool StudentWorld::initoverlap(double x, double y) const
{
    list<Actor*>::const_iterator it;

    for (it = m_actors.begin(); it != m_actors.end(); it++)
    {
        double dx = x - (*it)->getX();
        double dy = y - (*it)->getY();
        double distance = sqrt(dx*dx + dy*dy);
    
        if (distance < SPRITE_WIDTH && !(*it)->isdirt())
        {
            return true;
        }
    }
    return false;
}

bool StudentWorld::isoverlapping(double x, double y, const Actor *b) const
{
    double dx = x - b->getX();
    double dy = y - b->getY();
    double distance = sqrt(dx*dx + dy*dy);
    
    if (distance <= SPRITE_RADIUS)
    {
        return true;
    }
    return false;
}
