#include <algorithm>

#include "StudentWorld.h"

#include <cmath>
using namespace std;

///////
//////Actor implementation///
//////

Actor::Actor(int imageID, double startX, double startY, Direction dir, int depth, double size, StudentWorld* studentworld)
: GraphObject(imageID, startX, startY, dir, depth, size)
{
    m_StudentWorld = studentworld;
    m_isalive = true;
}


    //set actor to alive by default.
bool Actor::isalive()
{
    return m_isalive;
}

    //sets actor to dead
void Actor::die()
{
    m_isalive = false;
}

//returns a pointer to the studentworld
StudentWorld* Actor::getWorld()
{
    return m_StudentWorld;
}

//damagable actors have losehp, this is in the base class so that Studentworld can call losehp for derived classes with Actor pointers.
void Actor::losehp(double amt)
{
    return;
}
    //set to false by default.
bool Actor::isenemy()
{
    return false;
}



bool Actor::isdamageable()
{
    return false;
}

bool Actor::isedible()
{
    return false;
}

bool Actor::isdirt()
{
    return false;
}
    //set to false by default.
bool Actor::isprojectile()
{
    return false;
}

Actor::~Actor()
{
}


///////
//////DamageableActor implementation///
//////



DamageableActor::DamageableActor(int imageID, double startX, double startY, Direction dir, int depth, double size, StudentWorld* studentworld, double hp)
:Actor(imageID, startX, startY, dir, depth, size, studentworld)
{
    m_hp = hp;
}

double DamageableActor::hp()
{
    return m_hp;
}

    //only socrates can gain health in this game.
void DamageableActor::gainhp(double amt)
{
    m_hp += amt;
    if (m_hp > 100)
        m_hp = 100;
}

bool DamageableActor::isdamageable()
{
    return true;
}

    //losehp varies depending on actor
void DamageableActor::losehp(double amt)
{
    m_hp -= amt;
}

DamageableActor::~DamageableActor()
{
}



///////
//////Socrates implementation///
//////

Socrates::Socrates(StudentWorld* studentworld)
: DamageableActor(IID_PLAYER, 0, 128, 0, 0, 1, studentworld, 100)
//DamageableActor(<#int imageID#>, <#double startX#>, <#double startY#>, Direction dir, <#int depth#>, <#double size#>, <#StudentWorld *studentworld#>, <#double hp#>)
{
    m_spraycharges = MAXSPRAYS;
    m_flamecharges = MAXFLAMES;
}

void Socrates::doSomething()
{
    if (!isalive())
        return;
    int ch;
    if (getWorld() -> getKey(ch))
    {
        //user hit a key during this tick!
        switch (ch) {
            case KEY_PRESS_LEFT:
            {
                //move counterclockwise
                double prev_theta = (getDirection()+180) % 360;
                double new_theta = prev_theta + 5;
                double new_x,new_y;
                getWorld()->moveAroundCircle(new_theta, new_x, new_y);
                moveTo(new_x, new_y);
                setDirection(getDirection()+5);
                break;
            }
                
            case KEY_PRESS_RIGHT:
            {
                //move clockwise
                double prev_theta = (getDirection()+180) % 360;
                double new_theta = prev_theta - 5;
                double new_x,new_y;
                getWorld()->moveAroundCircle(new_theta, new_x, new_y);
                moveTo(new_x, new_y);
                setDirection(getDirection()-5);
                
                break;
            }
            case KEY_PRESS_SPACE:
            {
                if (spraycharges() > 0)
                {
                    //create sprayproj object
                    double dx, dy;
                    getPositionInThisDirection(getDirection(), SPRITE_RADIUS*2, dx, dy);
                    SprayProj* newSpray = new SprayProj(getWorld(), dx, dy , getDirection());
                    getWorld()->addactor(newSpray);
                    m_spraycharges--;
                    getWorld()->playSound(SOUND_PLAYER_SPRAY);
                    
                    
                }
                break;
            }
            case KEY_PRESS_ENTER:
            {
                if (flamecharges() > 0)
                {
                    //iterates and creates 16 flames all around socrates. the flame will expand and dissapate
                    int dir = 0;
                    for (int i = 0; i < 16; i++)
                    {
                        double dx, dy;
                        dir += 22.5;
                        getPositionInThisDirection(dir, SPRITE_RADIUS*2, dx, dy);
                        FlameProj* newflame = new FlameProj(getWorld(), dx, dy, dir);
                        getWorld()->addactor(newflame);
                        
                        getWorld()->playSound(SOUND_PLAYER_FIRE);
                        
                    }
                    m_flamecharges--;
                    
                }
                break;
            }

        }
    }
    
    //always increase number of spray charges by 1 if currentcount is less than max.
    if (spraycharges() < MAXSPRAYS)
        m_spraycharges++;

}

//void Socrates::moveAroundCircle(double angle, double& new_x, double& new_y)
//{
//    new_x = VIEW_RADIUS+VIEW_RADIUS*cos(angle*(4*atan(1)/180));
//    new_y = VIEW_RADIUS+VIEW_RADIUS*sin(angle*(4*atan(1)/180));
//
//}

int Socrates::spraycharges()
{
    return m_spraycharges;
}

int Socrates::flamecharges()
{
    return m_flamecharges;
}

void Socrates::increaseflamecharges()
{
    m_flamecharges += 5;
}

Socrates::~Socrates()
{
}


///////
//////Dirt implementation///
//////

Dirt::Dirt(StudentWorld* studentworld, double startX, double startY)
: DamageableActor(IID_DIRT, startX, startY, 0, 1, 1, studentworld, 0)
//int m_imageID, double startX, double startY, Direction Dirt, int m_depth, double size, StudentWorld *studentworld
{
}
    //dirt does nothing
void Dirt::doSomething()
{
}

bool Dirt::isdirt()
{
    return true;
}

Dirt::~Dirt()
{
}

///////
//////Food implementation///
//////

Food::Food(StudentWorld* studentworld, double startX, double startY)
: Actor(IID_FOOD, startX, startY, 0, 1, 1, studentworld)
{
}

void Food::doSomething()
{
}

bool Food::isedible()
{
    return true;
}

bool Food::isenemy()
{
    return true;
}

Food::~Food()
{
}

///////
//////Pit implementation///
//////

Pit::Pit(StudentWorld* studentworld, double startX, double startY)
: Actor(IID_PIT, startX, startY, 0, 1, 1, studentworld)
{
    m_regsalm = 5;
    m_aggsalm = 3;
    m_ecoli = 2;
}
void Pit::doSomething() // work on this later
{
    if(!isalive())
    {
        getWorld()->decpits();
        return;
    }
    
    if (randInt(1,50) == 1) // spawn a random bacteria
    {
        int typesAlive = 0;
        bool bacAlive[3] = {false,false,false};

        if (m_regsalm > 0)
        {
            typesAlive++;
            bacAlive[0] = true;
        }
        if (m_aggsalm > 0)
        {
            typesAlive++;
            bacAlive[1] = true;
        }
        if (m_ecoli > 0)
        {
            typesAlive++;
            bacAlive[2] = true;
        }
        
        int random = randInt(0, typesAlive-1);
        int curr = 0;
        for (int i = 0; i < 3; i++)
        {
            if (bacAlive[i] == true)
            {
                if (random == curr)
                {
                    if (i == 0) // spawn reg.
                    {
                        Actor* newbacteria = new RegSalmonella(getWorld(),getX(),getY(), 4, IID_SALMONELLA);
                        getWorld()->addactor(newbacteria);
                        m_regsalm--;
                    }
                    else if (i == 1) // spawn agg.
                    {
                        Actor* newbacteria = new AggSalmonella(getWorld(),getX(),getY());
                        getWorld()->addactor(newbacteria);
                        m_aggsalm--;
                    }
                    else if (i == 2) // spawn ecoli
                    {
                        Actor* newbacteria = new EColi(getWorld(),getX(),getY());
                        getWorld()->addactor(newbacteria);
                        m_ecoli--;
                    }
                    getWorld()->playSound(SOUND_BACTERIUM_BORN);
                    return;
                }
                curr++;
            }
        }
        
    }
}

bool Pit::isenemy()
{
    return true;
}

Pit::~Pit()
{
}

////////////////////////////////
//////GoodieS implementation///
////////////////////////////////

Goodie::Goodie(int imageID, double startX, double startY, StudentWorld* studentworld)
: DamageableActor(imageID, startX, startY, 0, 1, 1, studentworld, 0)
{
    m_lifetime = fmax( rand()%(300 - 10*(studentworld->getLevel())), 50);
}

bool Goodie::isexpired()
{
    if (m_lifetime <= 0)
        return true;
    
    return false;
}

void Goodie::checkcollision()
{
    
    if (getWorld()->isWithinEuclidianDistance(getX(), getY(), getWorld()->getSocrates()))  //the item collided with the Socrates
    {

        die();

        getWorld()->playSound(SOUND_GOT_GOODIE);

        applyeffect();
    }
}

void Goodie::doSomething()
{
    if (!isalive())
        return;
    
    checkcollision();

    reducelifetime();
    
    if (isexpired())
        die();
}

bool Goodie::isedible()
{
    return true;
}

void Goodie::reducelifetime()
{
    m_lifetime--;
}


Goodie::~Goodie()
{
}
   
////////////////////////////////
//////Restorehealthgoods implementation///
////////////////////////////////

RestorehealthGoods::RestorehealthGoods(StudentWorld* studentworld, double startX, double startY)
: Goodie(IID_RESTORE_HEALTH_GOODIE, startX, startY, studentworld)
{
}

void RestorehealthGoods::applyeffect()
{
    getWorld()->getSocrates()->gainhp(100);
    getWorld()->increaseScore(250);
    
}

RestorehealthGoods::~RestorehealthGoods()
{
}

////////////////////////////////
//////Flamethowergood implementation///
////////////////////////////////

FlameThrowerGoods::FlameThrowerGoods(StudentWorld* studentworld, double startX, double startY)
: Goodie(IID_FLAME_THROWER_GOODIE, startX, startY, studentworld)
{
}

void FlameThrowerGoods::applyeffect()
{
    getWorld()->getSocrates()->increaseflamecharges();
    getWorld()->increaseScore(300);
}

FlameThrowerGoods::~FlameThrowerGoods()
{
}

////////////////////////////////
//////Extralifegood implementation///
////////////////////////////////

ExtraLifeGoods::ExtraLifeGoods(StudentWorld* studentworld, double startX, double startY)
: Goodie(IID_EXTRA_LIFE_GOODIE, startX, startY, studentworld)
{
}

void ExtraLifeGoods::applyeffect()
{
    getWorld()->incLives();
    getWorld()->increaseScore(500);
}

ExtraLifeGoods::~ExtraLifeGoods()
{
}

////////////////////////////////
//////Fungus Implementation///
////////////////////////////////
Fungus::Fungus(StudentWorld* studentworld, double startX, double startY)
:Goodie(IID_FUNGUS, startX, startY, studentworld)
{
}

void Fungus::applyeffect()
{
    getWorld()->getSocrates()->losehp(20);
    if (getWorld()->getSocrates()->hp() <= 0)
    {
        getWorld()->getSocrates()->die();
        getWorld()->playSound(SOUND_PLAYER_DIE);
    }
    getWorld()->increaseScore(-50);
}

void Fungus::checkcollision()
{
    if (getWorld()->isWithinEuclidianDistance(getX(), getY(), getWorld()->getSocrates()))  //the item collided with the Socrates
    {
        die();

        getWorld()->playSound(SOUND_PLAYER_HURT);

        applyeffect();
    }
}

Fungus::~Fungus()
{
}
////////////////////////////////
//////Enemy implementation///
////////////////////////////////

Enemy::Enemy(int imageID, double startX, double startY, StudentWorld* studentworld, double hp)
: DamageableActor(imageID, startX, startY, 90, 0, 1, studentworld, hp)
{
    m_movementplandistance = 0;
    m_foodcounter = 0;
}

bool Enemy::isenemy()
{
    return true;
}

int Enemy::movementplandistance()
{
    return m_movementplandistance;
}

bool Enemy::moveandcheckisblocked()
{

    bool isblocked = false;
    
    double dx, dy;
    getPositionInThisDirection(getDirection(), 3, dx, dy);
    
    if (getWorld()->iscollidingdirt(dx, dy) || !getWorld()->isinbound(dx, dy, 0))
    {
        isblocked = true;
    }
    else
    {
        m_movementplandistance--;
        moveTo(dx, dy);
    }
    return isblocked;
        
//        if (isblocked)
//        {
//            blockmovement();
////            moveForward();
//        }

}

void Enemy::resetmovementplan()
{
    m_movementplandistance = 10;
}

void Enemy::blockmovement()
{
    setDirection(randInt(0, 359));
    resetmovementplan();
}

void Enemy::checkcollisionwithfood()
{
    if (getWorld()->iscollidingwithfood(getX(), getY()))
    {
        m_foodcounter++;
    }
}



void Enemy::findfood()
{
    
    if (getWorld()->foodisnearby(this)) //if food is nearby, set direction towards food
    {
        resetmovementplan();
    }
    else
    {
        blockmovement();
        
    }

}


bool Enemy::checkeatenfoods(double& newx, double& newy)
{
    if (m_foodcounter >= 3)
    {
        if (getX() < VIEW_WIDTH/2){
            newx = getX() + SPRITE_RADIUS;
        }
        if (getX() > VIEW_WIDTH/2){
            newx = getX() - SPRITE_RADIUS;
        }
        
        if (getY() < VIEW_WIDTH/2){
            newy = getY() + SPRITE_RADIUS;
        }
        if (getY() > VIEW_WIDTH/2){
            newy = getY() - SPRITE_RADIUS;
        }
        
        m_foodcounter = 0;
        
        return true;
    }
    return false;
}

Enemy::~Enemy()
{
}

////////////////////////////////
//////Regsalmonella implementation///
////////////////////////////////


RegSalmonella::RegSalmonella(StudentWorld* studentworld, double startX, double startY, int init_health, int imageID)
: Enemy(imageID, startX, startY, studentworld, init_health)
{
}

void RegSalmonella::losehp(double amt)
{
    DamageableActor::losehp(amt);

    getWorld()->playSound(SOUND_SALMONELLA_HURT);
    if (hp() <= 0 && isalive())
    {
        getWorld()->playSound(SOUND_SALMONELLA_DIE);
        getWorld()->increaseEnemiesKilled();
        getWorld()->increaseScore(100);
        die();
    }
}

bool RegSalmonella::doSomethingpart1(int losehealth, char typebacteria, bool skip)
{

    //2. check to see if colliding with socrates

    if (getWorld()->isWithinEuclidianDistance(getX(), getY(), getWorld()->getSocrates()))
    {
        getWorld()->playSound(SOUND_PLAYER_HURT);
        getWorld()->getSocrates()->losehp(losehealth);
        if (getWorld()->getSocrates()->hp() <= 0)
        {
            getWorld()->getSocrates()->die();
            getWorld()->playSound(SOUND_PLAYER_DIE);
        }
        
        if (skip)
            return true;
        
        if (movementplandistance()>0) {
            if (moveandcheckisblocked()) {
                blockmovement();
                return true;
            }
        }
        else {
            findfood();
            return true;
        }
    }
    
    //3. check to see if bacteria has eaten 3 foods
    double newx = getX(), newy = getY();
    if (checkeatenfoods(newx, newy))
    {
        getWorld()->playSound(SOUND_BACTERIUM_BORN);
        switch(typebacteria) //the type of bacteria will dictate what new bacteria is formed from three eaten pizzas
        {
            case 'R':
            {
                RegSalmonella* newbacterium = new RegSalmonella(getWorld(), newx, newy, 4, IID_SALMONELLA);
                getWorld()->addactor(newbacterium);
                break;
            }
                
            case 'A':
            {
                AggSalmonella* newbacterium = new AggSalmonella(getWorld(), newx, newy);
                getWorld()->addactor(newbacterium);
                break;
            }
                
            case 'E':
            {
                EColi* newbacterium = new EColi(getWorld(), newx, newy);
                getWorld()->addactor(newbacterium);
                break;
            }
            
        }
        getWorld()->increaseTargetkillcount();
        
        if (skip)
            return true;
        
        if (movementplandistance() > 0) {
            if (moveandcheckisblocked()){
                blockmovement();
                return true;
            }
        }
        else {
            findfood();
            return true;
        }
    }
    return false;

}

void RegSalmonella::doSomething()
{
    if (!isalive())
        return;
    
    if (doSomethingpart1(1, 'R', false)) //false--> because regsalmenella will take all courses of action listed in doSomethingpart1
        return;
    
    //4. check to see if colliding with food
    checkcollisionwithfood();

    //5+6. check movementdistanceplan MDP, if MPD > 0, moveforward. if blocked change direction
    //if MDP = 0, findfood, if blocked, change direction.
    if (movementplandistance() > 0){
        if(moveandcheckisblocked()){
            blockmovement();
            return;
        }
    }
    else {
        findfood();
        return;
    }
    
}
 
RegSalmonella::~RegSalmonella()
{
}

////////////////////////////////
//////AggSalmonella implementation///
////////////////////////////////

AggSalmonella::AggSalmonella(StudentWorld* studentworld, double startX, double startY)
: RegSalmonella(studentworld, startX, startY, 10, IID_SALMONELLA)
{
}
    
void AggSalmonella::doSomething()
{
    if (!isalive())
        return;
    
    if(getWorld()->socratesisnearby(this, 78))
    {
        resetmovementplan();
        //move towards socrates
        
        if (moveandcheckisblocked()){
//            cout << 'R' << endl;
            //dont change direction when collides with dirt,
            
            if(doSomethingpart1(2, 'A', true))
                return;
            
            //check to see if colliding with food
            checkcollisionwithfood();

        }
        return;
    }
    
    //step 3.
    if (doSomethingpart1(2, 'A', false))
        return;
    
    //step 5. check to see if colliding with food
    checkcollisionwithfood();
    
    //step 6
    if (movementplandistance() > 0) {
        if (moveandcheckisblocked()){
            
            blockmovement();
            return;
        }
    }
    else {
        findfood();
        return;
    }

}
    
AggSalmonella::~AggSalmonella()
{}

////////////////////////////////
//////Ecoli implementation///
////////////////////////////////
EColi::EColi(StudentWorld* studentworld, double startX, double startY)
: RegSalmonella(studentworld, startX, startY, 5, IID_ECOLI)
{
}

void EColi::losehp(double amt)
{
    DamageableActor::losehp(amt);

    getWorld()->playSound(SOUND_ECOLI_HURT);
    if (hp() <= 0 && isalive())
    {
        getWorld()->playSound(SOUND_ECOLI_DIE);
        getWorld()->increaseEnemiesKilled();
        getWorld()->increaseScore(100);
        die();
    }
}

void EColi::doSomething()
{
    if (!isalive())
    {
        return;
    }
    
    if(doSomethingpart1(4, 'E', false))
        return;
    
    //4. check to see if colliding with food
    checkcollisionwithfood();
    
    if(getWorld()->socratesisnearby(this, 256))
    {
        for (int i = 0; i < 10; i++)
        {
            if (moveandcheckisblocked())
            {
                if (getDirection()+10 > 360)
                {
                    int newtheta = 10 - (360 - getDirection());
                    setDirection(newtheta);
                }
                setDirection(getDirection() + 10);
                
            }
            else
                return;
        }
    }
    
}
    
EColi::~EColi()
{
}

////////////////////////////////
//////projectile implementation///
////////////////////////////////
Projectile::Projectile(int imageID, double startX, double startY, Direction dir, StudentWorld* studentworld, int movementdistance)
: Actor(imageID, startX, startY, dir, 1, 1, studentworld)
{
    m_movementplandistance = movementdistance;
}

void Projectile::doSomething()
{
    double dx, dy;
    getPositionInThisDirection(getDirection(), SPRITE_RADIUS*2, dx, dy);
    moveTo(dx, dy);
    decmovementdistance();
    
    if (movementdistance() <= 0)
    {
        die();
    }
}

bool Projectile::isprojectile()
{
    return true;
}

void Projectile::decmovementdistance()
{
    m_movementplandistance -= SPRITE_RADIUS*2;
}

int Projectile::movementdistance()
{
    return m_movementplandistance;
}


bool Projectile::checkcollision(int losehealth)
{
    if(getWorld()->iscollidingprojectile(getX(), getY(), losehealth) && isalive())
    {
        die();
        return true;
    }
    return false;
}

Projectile::~Projectile()
{
}

////////////////////////////////
//////FLAME PROJECTILE CLASS DECLARATION///
////////////////////////////////


FlameProj::FlameProj(StudentWorld *studentworld, double startX, double startY, Direction dir)
:Projectile(IID_FLAME, startX, startY, dir, studentworld, 32)
{

}

void FlameProj::doSomething()
{
    if (!isalive())
        return;
    
    if (checkcollision(5))
        return;
    
    Projectile::doSomething();
    
}

FlameProj::~FlameProj()
{
}

////////////////////////////////
//////Sprayimplementation///
////////////////////////////////

SprayProj::SprayProj(StudentWorld *studentworld, double startX, double startY, Direction dir)
: Projectile(IID_SPRAY, startX, startY, dir, studentworld, 112)
{
}

void SprayProj::doSomething()
{
    if (!isalive())
        return;
    
    if (checkcollision(2))
        return;
    
    Projectile::doSomething();
}

SprayProj::~SprayProj()
{
}



