#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <iostream>
#include <string>

using namespace std;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
//    GraphObject(int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0)
//constants
const int INITIALHP = 100;
const int RIGHT = 0;
const int LEFT = 180;
const int DOWNANDLEFT = 225;
const int UPANDLEFT = 135;
const int MAXSPRAYS = 20;
const int MAXFLAMES = 5;

class StudentWorld;

////////////////////////////////
//////ACTOR CLASS DECLARATION///
////////////////////////////////

class Actor : public GraphObject
{
public:
    Actor(int imageID, double startX, double startY, Direction dir, int depth, double size, StudentWorld* studentworld);

    //pure-virtual :all actors doSomething different, an actor on its own doesnt do anything
    virtual void doSomething() = 0;

    //set actor to alive by default.
    bool isalive();

    //sets actor to dead
    void die();

    StudentWorld* getWorld();

    //set to false by default.
    virtual bool isenemy();
    
    virtual bool isdamageable();
    
    virtual bool isedible();
    
    virtual bool isdirt();
    
    virtual void losehp(double amt);

    //set to false by default.
    virtual bool isprojectile();
                        
    
    //TODO: create virtual blockmovement(); 

    virtual ~Actor();
    

private:
    StudentWorld* m_StudentWorld;
    
    

    bool m_isalive;

};

////////////////////////////////
//////DAMAGEABLE CLASS DECLARATION///
////////////////////////////////

class DamageableActor : public Actor
{
public:
    DamageableActor(int imageID, double startX, double startY, Direction dir, int depth, double size, StudentWorld* studentworld, double hp);

    double hp();

    ///increase our decrease healthpoints, socrates loses hp when attacked by bacteria(depends on bacteria) or steps on fungus (-20hp), gains hp when steps on healthgoodies
    ///bacteria loses health when hit by projectiles.

    //only socrates can gain health in this game.
    void gainhp(double amt);

    virtual bool isdamageable();
    //losehp varies depending on actor
    virtual void losehp(double amt);

    virtual ~DamageableActor();
    
    

private:
    double m_hp; //socrates starts out with 100 HP, regsalm 4HP,aggsalm 10HP, ecoli 5HP

};



////////////////////////////////
//////SOCRATES CLASS DECLARATION///
////////////////////////////////

class Socrates : public DamageableActor
{
public:
    Socrates(StudentWorld* studentworld);

    virtual void doSomething();

    int spraycharges();

    int flamecharges();

    void increaseflamecharges();
    
    
//    void moveAroundCircle(double angle, double& new_x, double& new_y);

    virtual ~Socrates();

private:
    int m_spraycharges;
    int m_flamecharges;
};

////////////////////////////////
//////DIRT CLASS DECLARATION///
////////////////////////////////

class Dirt : public DamageableActor
{
public:
    Dirt(StudentWorld* studentworld, double startX, double startY);

    //dirt does nothing
    virtual void doSomething();
    
    virtual bool isdirt();

    virtual ~Dirt();
};

////////////////////////////////
//////FOOD CLASS DECLARATION///
////////////////////////////////

class Food : public Actor
{
public:
    Food(StudentWorld* studentworld, double startX, double startY);

    virtual void doSomething();
    
    virtual bool isedible();
    
    virtual bool isenemy();

    virtual ~Food();
};

////////////////////////////////
//////PIT CLASS DECLARATION///
////////////////////////////////

class Pit : public Actor
{
public:
    Pit(StudentWorld* studentworld, double startX, double startY);
    
    virtual void doSomething();
    
    virtual bool isenemy();
    
    virtual ~Pit();
private:
    int m_regsalm;
    int m_aggsalm;
    int m_ecoli;
    
};

////////////////////////////////
//////ENEMY CLASS DECLARATION///
////////////////////////////////
class Enemy : public DamageableActor
{
public:
    Enemy(int imageID, double startX, double startY, StudentWorld* studentworld, double hp);

    virtual bool isenemy();
    
    virtual int movementplandistance();
    
    virtual void resetmovementplan();
    
    virtual ~Enemy();

    
protected:

    void checkcollisionwithfood();
    
    bool moveandcheckisblocked();
    
    void blockmovement();
    
    bool checkeatenfoods(double& newx, double& newy);
    
    void findfood();
    
private:
    
    int m_foodcounter;
    
    int m_movementplandistance;

};


                        
////////////////////////////////
//////REGULAR SALMONELLA CLASS DECLARATION///
////////////////////////////////

class RegSalmonella : public Enemy
{
public:
    RegSalmonella(StudentWorld* studentworld, double startX, double startY, int init_health, int imageID);
    
    virtual void doSomething();
    
    virtual void losehp(double amt);
    
    virtual bool doSomethingpart1(int losehealth, char typebacteria, bool skip);
    
    virtual ~RegSalmonella();
};

class AggSalmonella : public RegSalmonella
{
public:
    AggSalmonella(StudentWorld* studentworld, double startX, double startY);
    
    virtual void doSomething();
    
    virtual ~AggSalmonella();
};

class EColi : public RegSalmonella
{
public:
    EColi(StudentWorld* studentworld, double startX, double startY);
    
    virtual void losehp(double amt);
    
    virtual void doSomething();
    
    virtual ~EColi();
};





////////////////////////////////
//////Goodie CLASS DECLARATION///
////////////////////////////////

class Goodie : public DamageableActor
{
public:
    Goodie(int imageID, double startX, double startY, StudentWorld* studentworld);
    
    virtual bool isexpired();
    
    virtual ~Goodie();
    
    virtual bool isedible();
    
    virtual void doSomething();
    
    virtual void reducelifetime();
    
protected:
    virtual void checkcollision();
    
    virtual void applyeffect() = 0;
private:
    double m_lifetime;
    

    
};

///
////////////////////////////////
//////RESTOREHEALTHGOODIES  CLASS DECLARATION///
////////////////////////////////
class RestorehealthGoods : public Goodie
{
public:
    RestorehealthGoods(StudentWorld* studentworld, double startX, double startY);
    
    virtual ~RestorehealthGoods();
    
private:
    virtual void applyeffect();
};
////////////////////////////////
//////FLAMETHROWERGOODIES  CLASS DECLARATION///
////////////////////////////////
class FlameThrowerGoods : public Goodie
{
public:
    FlameThrowerGoods(StudentWorld* studentworld, double startX, double startY);
    
    virtual ~FlameThrowerGoods();
    
private:
    virtual void applyeffect();
    
};

////////////////////////////////
//////EXTRALIFEGOODIES  CLASS DECLARATION///
////////////////////////////////

class ExtraLifeGoods : public Goodie
{
public:
    ExtraLifeGoods(StudentWorld* studentworld, double startX, double startY);
    
    virtual ~ExtraLifeGoods();

private:
    virtual void applyeffect();
};

////////////////////////////////
//////Fungus CLASS DECLARATION///
////////////////////////////////
class Fungus : public Goodie
{
public:
    Fungus(StudentWorld* studentworld, double startX, double startY);
    
    virtual ~Fungus();
    
protected:
    virtual void checkcollision();
    
private:
    virtual void applyeffect();
};


////////////////////////////////
//////PROJECTILE CLASS DECLARATION///
////////////////////////////////
class Projectile : public Actor
{
public:
    Projectile(int imageID, double startX, double startY, Direction dir, StudentWorld* studentworld, int movementdistance);
    
    virtual bool isprojectile();
    
    virtual void doSomething();

    virtual ~Projectile();
    
    void decmovementdistance();
    
    int movementdistance();
    
protected:
    
    virtual bool checkcollision(int losehealth);
    
private:
    
    int m_movementplandistance;

};

////////////////////////////////
//////FLAME PROJECTILE CLASS DECLARATION///
////////////////////////////////

class FlameProj : public Projectile
{
public:
    FlameProj(StudentWorld *studentworld, double startX, double startY, Direction dir);

    virtual void doSomething();

    virtual ~FlameProj();
    
};

////////////////////////////////
//////DISINFECTANT SPRAY PROJECTILE CLASS DECLARATION///
////////////////////////////////

class SprayProj : public Projectile
{
public:
    SprayProj(StudentWorld *studentworld, double startX, double startY, Direction dir);
    
    virtual void doSomething();
    
    virtual ~SprayProj();
};




#endif // ACTOR_H_
