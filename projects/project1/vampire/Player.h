//
//  Player.h
//  vampire2
//
//  Created by Rodger Lee on 1/9/20.
//  Copyright © 2020 Rodger Lee. All rights reserved.
//

#ifndef Player_h
#define Player_h

class Arena;

class Player
{
  public:
      // Constructor
    Player(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    string dropPoisonVial();
    string move(int dir);
    void   setDead();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
};
#endif /* Player_h */
