//
//  newMap.cpp
//  Homework1
//
//  Created by Rodger Lee on 1/15/20.
//  Copyright © 2020 Rodger Lee. All rights reserved.
//

#include <stdio.h>
#include "newMap.h"
#include <iostream>
#include <string>

using namespace std;

Map::Map()
: m_size(0)
{
    m_size = 0;
    m_maxsize = DEFAULT_MAX_ITEMS;
    m_pairs = new KeyValuePair[m_maxsize];
}// Create an empty map (i.e., one with no key/value pairs)

Map::Map(int maxsize)
: m_size(0)
{
    m_maxsize = maxsize;
    m_pairs = new KeyValuePair[m_maxsize];
}

Map::~Map()
{
    delete [] m_pairs;
}

Map::Map(const Map &src)
{
    m_pairs = new KeyValuePair[m_maxsize];
    m_size = src.m_size;
    m_maxsize = src.m_maxsize;
    for (int i = 0; i < m_maxsize; i++)
    {
        m_pairs[i] = src.m_pairs[i];
    }
    
}

Map &Map::operator=(const Map &src)
{
    if (&src == this)
    {
        return *this;
    }
    delete [] m_pairs;
    m_size = src.m_size;
    m_maxsize = src.m_maxsize;
    m_pairs = new KeyValuePair[m_maxsize];
    for (int i = 0; i < m_maxsize; i++)
    {
        m_pairs[i] = src.m_pairs[i];
    }
    return *this;
}


bool Map::empty() const
{
    if (m_size == 0)
        return true;
    return false;
}// Return true if the map is empty, otherwise false.

int Map::size() const
{
    return m_size;
}// Return the number of key/value pairs in the map.

bool Map::insert(const KeyType& key, const ValueType& value)
{
    if (contains(key) || m_size >= m_maxsize)
        return false;
    
    m_pairs[m_size].key = key;
    m_pairs[m_size].value = value;
    
    m_size++;
    return true;
}
  // If key is not equal to any key currently in the map, and if the
  // key/value pair can be added to the map, then do so and return true.
  // Otherwise, make no change to the map and return false (indicating
  // that either the key is already in the map, or the map has a fixed
  // capacity and is full).

bool Map::update(const KeyType& key, const ValueType& value)
{
    if (!contains(key))
        return false;
    m_pairs[currentpair(key)].value = value;
    return true;
}
  // If key is equal to a key currently in the map, then make that key no
  // longer map to the value it currently maps to, but instead map to
  // the value of the second parameter; return true in this case.
  // Otherwise, make no change to the map and return false.

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    if (m_size >= m_maxsize)
        return false;
    if (contains(key))
    {
        update(key, value);
        return true;
    }
    insert(key, value);
    return true;
    
}
  // If key is equal to a key currently in the map, then make that key no
  // longer map to the value it currently maps to, but instead map to
  // the value of the second parameter; return true in this case.
  // If key is not equal to any key currently in the map and if the
  // key/value pair can be added to the map, then do so and return true.
  // Otherwise, make no change to the map and return false (indicating
  // that the key is not already in the map and the map has a fixed
  // capacity and is full).

bool Map::erase(const KeyType& key)
{
    if (!contains(key))
        return false;
    m_pairs[m_size] = m_pairs[currentpair(key)];
    m_size--;
    return true;
}
  // If key is equal to a key currently in the map, remove the key/value
  // pair with that key from the map and return true.  Otherwise, make
  // no change to the map and return false.
 
bool Map::contains(const KeyType& key) const
{
    for (int i = 0; i < m_size; i++)
    {
        if (m_pairs[i].key == key)
            return true;
    }
    return false;
}
  // Return true if key is equal to a key currently in the map, otherwise
  // false.
 
bool Map::get(const KeyType& key, ValueType& value) const
{
    if (!contains(key))
        return false;
    value = m_pairs[currentpair(key)].value;
    return true;
}
  // If key is equal to a key currently in the map, set value to the
  // value in the map that that key maps to, and return true.  Otherwise,
  // make no change to the value parameter of this function and return
  // false.
 
bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if (0 > i || i >= size())
        return false;
    key = m_pairs[i].key;
    value = m_pairs[i].value;
    return true;
    
}
  // If 0 <= i < size(), copy into the key and value parameters the
  // key and value of one of the key/value pairs in the map and return
  // true.  Otherwise, leave the key and value parameters unchanged and
  // return false.  (See below for details about this function.)

void Map::swap(Map& other)
{
    Map temp = *this;
    *this = other;
    other = temp;
    
//        for (int i = 0; i < size(); i++) // fill in other(temp) with target map(this)
//        {
//            other.m_pairs[i] = m_pairs[i];
//        }
//        other.m_size = size();
//        for (int i = 0; i < temp.size(); i++)
//        {
//            m_pairs[i] = temp.m_pairs[i];
//        }
//        m_size = temp.size();
    
}
  // Exchange the contents of this map with the other one.

int Map::currentpair(const KeyType &key) const
{
    if (!contains(key))
        return -1;
    for (int i = 0; i < m_size; i++)
    {
        if (m_pairs[i].key == key)
            return i;
    }
    return -1;
}
