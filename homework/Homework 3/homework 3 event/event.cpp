//
//  event.cpp
//  homework 3
//
//  Created by Rodger Lee on 2/5/20.
//  Copyright © 2020 Rodger Lee. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;

//Your declarations and implementations would go here

class Event
{
public:
    Event(string eventN)
    {
        m_eventname = eventN;
    }
    
    // virtual destructor
    virtual ~Event() {}
    
    //non-virtual function since all classes will use the same call name function
    string name() const
    {
        return m_eventname;
    }
    
    //non-pure virtual isSport is set to false and changed in other classes if true
    virtual bool isSport() const
    {
        return true;
    }
    
    //pure-virtual function need is never used by class Event
    virtual string need() const = 0;
    
private:
    string m_eventname;

};


//==============================
class BasketballGame : public Event
{
public:
    BasketballGame(string gameN) : Event(gameN)
    {
    }
    
    virtual ~BasketballGame()
    {
        cout << "Destroying the " << name() << " basketball game" << endl;
    }

    virtual string need() const
    {
        return "hoops";
    }
};


//==============================
class HockeyGame : public Event
{
public:
    HockeyGame(string gameN) : Event(gameN)
    {
    }
    virtual ~HockeyGame()
    {
        cout << "Destroying the " << name() << " hockey game" << endl;
    }

    virtual string need() const
    {
        return "ice";
    }
};


//==============================
class Concert : public Event
{
public:
    Concert(string band, string genre) : Event(band)
    {
        m_genre = genre;
    }
    
    virtual ~Concert()
    {
        cout << "Destroying the " << name() << " " << m_genre << " concert" << endl;
    }
    
    virtual bool isSport() const
    {
        return false;
    }
    
    virtual string need() const
    {
        return "a stage";
    }
private:
    string m_genre;
};



//=======================


void display(const Event* e)
{
    cout << e->name() << ": ";
    if (e->isSport())
    cout << "(sport) ";
    cout << "needs " << e->need() << endl;
}


//Here are the events.
//Lakers vs. Suns: (sport) needs hoops
//Banda MS: needs a stage
//KISS: needs a stage
//Kings vs. Flames: (sport) needs ice
//Cleaning up.
//Destroying the Lakers vs. Suns basketball game
//Destroying the Banda MS banda concert
//Destroying the KISS hard rock concert
//Destroying the Kings vs. Flames hockey game
