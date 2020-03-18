//
//  maze.cpp
//  homework 3 maze
//
//  Created by Rodger Lee on 2/8/20.
//  Copyright © 2020 Rodger Lee. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;


bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    if (sr == er && sc == ec)
        return true;
    
  // mark visited
    maze[sr][sc] = '@';
    
    //check E,N,w,S
    if (maze[sr][sc + 1] == '.' && pathExists(maze, nRows, nCols, sr, sc + 1, er, ec))
    {
            return true;
    }

    if (maze[sr - 1][sc] == '.' && pathExists(maze, nRows, nCols, sr - 1, sc, er, ec))
    {
            return true;
    }
    if (maze[sr][sc - 1] == '.' && pathExists(maze, nRows, nCols, sr, sc - 1, er, ec))
    {
            return true;
    }
    if (maze[sr + 1][sc] == '.' && pathExists(maze, nRows, nCols, sr + 1, sc, er, ec))
    {
            return true;
    }
    return false;
}

  // Return true if there is a path from (sr,sc) to (er,ec)
  // through the maze; return false otherwise


//If the start location is equal to the ending location, then we've
//    solved the maze, so return true.
//Mark the start location as visted.
//For each of the four directions,
//    If the location one step in that direction (from the start
//    location) is unvisited,
//        then call pathExists starting from that location (and
//            ending at the same ending location as in the
//            current call).
//         If that returned true,
//             then return true.
//Return false.
