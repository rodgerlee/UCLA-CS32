
#include <iostream>
using namespace std;

#include "globals.h"
#include "Arena.h"
#include "Vampire.h"
#include "Game.h"
#include "History.h"

///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

int main()
{
      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 5, 2);
    Game g(3, 5, 2);

      // Play the game
    g.play();

}

