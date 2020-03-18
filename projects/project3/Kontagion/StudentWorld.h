#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "GameWorld.h"
#include <string>
#include <list>

class Actor;
class Socrates;


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp


class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    virtual ~StudentWorld();
    
    //public helper functions
    Socrates* getSocrates() const;
    
    void addactor(Actor* a);
    
//    bool iscollidingSocrates(const Actor* a) const;
    
    bool iscollidingdirt(double x, double y) const;
    
    bool iscollidingwithfood(double x, double y);
    
    bool iscollidingActor(double x, double y) const;
    
    bool iscollidingprojectile(double x, double y, int losehealth);
    
    bool isinbound(double x, double y, int type) const;
    
    bool foodisnearby(Actor* a);
    
    bool socratesisnearby(Actor* a, int range);
    
    int setdirectiontotarget(const Actor* a, const Actor* b) const;
    
    void moveAroundCircle(double angle, double& new_x, double& new_y);
    
    bool isWithinEuclidianDistance(double x, double y, const Actor* b) const;
    
    void increaseEnemiesKilled();
    
    void increaseTargetkillcount();
    
    void decpits();
    
private:
    
    //private members
    std::list<Actor*> m_actors;
    
    Socrates* m_socrates;
    
    int m_numenemieskilled;
    
    int m_targetkillcount;
    
    //private helperfunctions
    bool socrateswon();
    
    void removedeadobjects();
    
    void addnewactors();
    
    void updatedisplaytext();
    
    
    bool initoverlap(double x, double y) const;
    
    bool isoverlapping(double x, double y, const Actor* b) const;
    
    int m_pits;
    
    
    
};

#endif // STUDENTWORLD_H_
