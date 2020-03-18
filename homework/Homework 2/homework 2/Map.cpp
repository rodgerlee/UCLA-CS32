//
//  Map.cpp
//  Project2
//
//  Created by Rodger Lee on 1/22/20.
//  Copyright © 2020 Rodger Lee. All rights reserved.
//

#include <stdio.h>

#include <iostream>
#include <string>
#include "Map.h"

using namespace std;


Map::Map()
: head(nullptr), m_size(0) // initializer list to start at empty or null
{
}

Map::~Map()
{
    deletenodes();
}
    

Map::Map(const Map &src)
{
    m_size = 0;
    Node *p = src.head;
    head = nullptr;
    while (p != nullptr)
    {
        insert(p->key, p->value);
        p = p->next;
    }
}

Map &Map::operator=(const Map &src)
{
    if (this == &src)
        return *this;
    
    deletenodes();   //delete all existing nodes and copy over
    
    m_size = 0;
    Node *p = src.head;
    head = nullptr;
    while (p != nullptr)
    {
        insert(p->key, p->value);
        p = p->next;
    }
    return *this;
}



bool Map::empty() const
{
    if (head == nullptr)
        return true;
    return false;
}

int Map::size() const
{
    return m_size;
}


bool Map::insert(const KeyType& key, const ValueType& value)
{
    if (contains(key))
        return false;
    
    //add to the top
    //special condition to append to the list when it is empty
    if (empty())
    {
        Node *p = new Node;
        p->key = key;
        p->value = value;
        p->next = nullptr;
        p->prev = nullptr;
        head = p;
        m_size++;
        return true;
    }
    // this appends nodes to the list if it is not empty
    m_size++;
    Node *p = new Node;
    p->key = key;
    p->value = value;
    p->next = head;
    p->prev = nullptr;
    head->prev = p;
    head = p;
   
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
    if (!contains(key))
        return false;
    Node *n = head;
    while (n->key != key && n != nullptr)
    {
        n = n->next;
    }
    
    n->value = value;
    return true;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    if (!contains(key))
        insert(key, value);
    update(key, value);
    return true;
    
}
bool Map::erase(const KeyType& key)
{
    if (!contains(key))
        return false;
    if (head->key == key) //if first node is the one we want
    {
        Node *k = head;
        head = k->next;
        k->next->prev = nullptr;
        delete k;
        m_size--;
        return true;
    }
    
    Node *n = head;
    while (n != nullptr) //traverse, and stop when pointer is pointing past the last node
    {
        if ((n->next != nullptr) && (n->next->key == key))
            break;
        n = n->next;
    }
    Node *erase = n->next;
    n->next = erase->next;
    if (erase->next != nullptr)
        erase->next->prev = n;
    delete erase;
    m_size--;
    
    return true;
}
bool Map::contains(const KeyType& key) const
{
    if (head == nullptr)
        return false;
    Node *p = head;
    
    //traverse through list
    while (p != nullptr)
    {
        if (p->key == key)
            return true;
        p = p->next;
    }
    return false;
    
}
bool Map::get(const KeyType& key, ValueType& value) const
{
    Node *n = head;
    
    //traverse to get value
    while (n != nullptr)
    {
        if (n->key == key)
        {
            value = n->value;
            return true;
        }
        n = n->next;
    }
    return false;
}
bool Map::get(int i, KeyType& key, ValueType& value) const
{
     if (0 > i || i >= size())
         return false;
    
    Node *n = head;
    
    //count matches the index i which begins at 0
    int count = 0;
    while (count != i)
    {
        n = n->next;
        count++;
    }
    key = n->key;
    value = n->value;
    return true;
    
}

void Map::swap(Map& other)
{
    
    if (this == &other)
        return;
    
    //a temphead must be created to switch head pointers
    Node *temphead = head;
    head = other.head;
    other.head = temphead;
    
    //trade other member variables
    int tempsize = m_size;
    m_size = other.m_size;
    other.m_size = tempsize;
    
//    Map temp = *this;
//    *this = other;
//    other = temp;
}
        
void Map::deletenodes()
{
    Node *p = head;
    
    //traverse to delete all nodes
    while (p != nullptr)
    {
        Node *temp = p->next;
        delete p;
        p = temp;
    }
    
    //set head to nullptr to indicate empty map
    head = nullptr;
}

        
//non-member functions
bool combine(const Map &m1, const Map &m2, Map &result)
{
    //create temp variables
    bool endresult = true;
    Map tempresult;
    KeyType key;
    ValueType value;
    
    //fill in new map tempresult with all of the existing nodes in m1
    for (int i = 0; i < m1.size(); i++)
    {
        m1.get(i, key, value);
        tempresult.insert(key, value);
    }
    
    //fill in new map tempresult by appending m2 nodes, check conditions in the meantime
    for (int j = 0; j < m2.size(); j++)
    {
        m2.get(j, key, value);
        
        //if tempresult already has this key, or a duplicate, check conditions
        if (tempresult.contains(key))
        {
            ValueType tempval;
            tempresult.get(key, tempval);
            
            //if the duplicate node has a corresponding value that is not equal to the value from m2. this function returns false, and we erase that existing key.
            if (tempval != value)
            {
                endresult = false;
                tempresult.erase(key);
            }
            continue;
        }
        tempresult.insert(key, value);
    }
    
    result = tempresult;
    return endresult;
}


void reassign(const Map &m, Map &result)
{
    //create temporary variables
    Map tempresult;
    KeyType key1;
    ValueType value1;
    KeyType key2;
    ValueType value2;
    
    //move each node's value 1 node up , (ie. G40 H45 I50 becomes G45 H50 I40 )
    for (int i = 0; i < m.size(); i++)
    {
        m.get(i, key1, value1);
        if (i < m.size()-1)
        {
            m.get(i+1, key2, value2);
            tempresult.insert(key1, value2);
            continue;
        }
        m.get(0, key2, value2);
        tempresult.insert(key1, value2);
    }
    result = tempresult;
}


void print(const Map &src)
{
    KeyType key;
    ValueType value;
    for (int i = 0; i < src.size(); i++)
    {
        src.get(i, key, value);
        cout << key << "" << value << endl;
    }
}
