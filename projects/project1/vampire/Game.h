//
//  Game.h
//  vampire2
//
//  Created by Rodger Lee on 1/9/20.
//  Copyright © 2020 Rodger Lee. All rights reserved.
//

#ifndef Game_h
#define Game_h

class Arena;

class Game
{
  public:
      // Constructor/destructor
    Game(int rows, int cols, int nVampires);
    ~Game();

      // Mutators
    void play();

  private:
    Arena* m_arena;

      // Helper functions
    string takePlayerTurn();
};


#endif /* Game_h */
