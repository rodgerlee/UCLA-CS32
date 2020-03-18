//
//  list.cpp
//  Homework 5
//
//  Created by Rodger Lee on 3/8/20.
//  Copyright © 2020 Rodger Lee. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Class
{
  public:
    Class(string nm) : m_name(nm) {}
    string name() const { return m_name; }
    const vector<Class*>& subclasses() const { return m_subclasses; }
    void add(Class* d) { m_subclasses.push_back(d); }
    ~Class();
  private:
    string m_name;
    vector<Class*> m_subclasses;
};

Class::~Class()
{
    for (size_t k = 0; k < m_subclasses.size(); k++)
        delete m_subclasses[k];
}

void listAll(string path, const Class* c)  // two-parameter overload
{
    
    if (path != "")
    {
        path = path + "=>" + c->name();
    }
    else
        path += c->name();
    
    cout << path << endl;
    
    vector<Class*>::const_iterator it = c->subclasses().begin(); //if there are subclasses, recurse through the subclasses
    while (it != c->subclasses().end())
    {
        listAll(path, (*it));
        it++;
    }

}

void listAll(const Class* c)  // one-parameter overload
{
    if (c != nullptr)
        listAll("", c);
}

int main()
{
    Class* d1 = new Class("Salmonella");
    listAll(d1);
    cout << "====" << endl;
    d1->add(new Class("AggressiveSalmonella"));
    Class* d2 = new Class("Bacterium");
    d2->add(new Class("EColi"));
    d2->add(d1);
    listAll(d2);
    cout << "====" << endl;
    Class* d3 = new Class("Goodie");
    d3->add(new Class("RestoreHealthGoodie"));
    d3->add(new Class("FlamethrowerGoodie"));
    d3->add(new Class("ExtraLifeGoodie"));
    listAll(d3);
    cout << "====" << endl;
    Class* d4 = new Class("Actor");
    d4->add(d2);
    d4->add(new Class("Flame"));
    d4->add(d3);
    listAll(d4);
    delete d4;
}


//OUTPUT:

//Salmonella
//====
//Goodie
//Goodie=>RestoreHealthGoodie
//Goodie=>FlamethrowerGoodie
//Goodie=>ExtraLifeGoodie
//====
//Actor
//Actor=>Bacterium
//Actor=>Bacterium=>EColi
//Actor=>Bacterium=>Salmonella
//Actor=>Bacterium=>Salmonella=>AggressiveSalmonella
//Actor=>Flame
//Actor=>Goodie
//Actor=>Goodie=>RestoreHealthGoodie
//Actor=>Goodie=>FlamethrowerGoodie
//Actor=>Goodie=>ExtraLifeGoodie
