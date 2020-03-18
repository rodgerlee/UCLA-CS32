//
//  Map.h
//  Project2
//
//  Created by Rodger Lee on 1/22/20.
//  Copyright © 2020 Rodger Lee. All rights reserved.
//

#ifndef Map_h
#define Map_h
#include <string>

using KeyType = char;
using ValueType = int;

class Map
{
  public:
    Map();
    ~Map();
    Map(const Map &src);
    Map &operator=(const Map &src);
    bool empty() const;
    int size() const;
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    void swap(Map& other);
    
  private:
    struct Node
    {
        KeyType key;
        ValueType value;
        Node *next;
        Node *prev;
    };
    Node* head;
    int m_size;
    
    void deletenodes();

};

//map algorithms non-member functions
bool combine(const Map& m1, const Map& m2, Map& result);
void reassign(const Map& m, Map& result);

void print(const Map& src);

#endif /* Map_h */
