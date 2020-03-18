//
//  mazestack.cpp
//  homework 2
//
//  Created by Rodger Lee on 2/1/20.
//  Copyright © 2020 Rodger Lee. All rights reserved.
//

#include <stdio.h>
#include <string>
#include <iostream>
#include <stack>

using namespace std;

class Coord
{
  public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
  private:
    int m_r;
    int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
 //Push the starting coordinate (sr,sc) onto the coordinate stack and
 //    update maze[sr][sc] to indicate that the algorithm has encountered
 //    it (i.e., set maze[sr][sc] to have a value other than '.').
    stack<Coord> coordstack;
    Coord initcoord(sr, sc);
    coordstack.push(initcoord);
    maze[sr][sc] = 'X';
    while (!coordstack.empty())
    {
        //{   Pop the top coordinate off the stack. This gives you the current
        //        (r,c) location that your algorithm is exploring.
        Coord temp = coordstack.top();
        coordstack.pop();
        int tempr = temp.r();
        int tempc = temp.c();
        //    If the current (r,c) coordinate is equal to the ending coordinate,
        //        then we've solved the maze so return true!
        if (tempr == er && tempc == ec)
            return true;

        //check each direction : E,S, W, N

        //        If you can move EAST and haven't encountered that cell yet,
        //            then push the coordinate (r,c+1) onto the stack and update
        //            maze[r][c+1] to indicate the algorithm has encountered it.
        if (maze[tempr][tempc + 1] == '.')
        {
            coordstack.push(Coord(tempr, tempc + 1));
            maze[tempr][tempc + 1] = 'X';
        }
        //        If you can move SOUTH and haven't encountered that cell yet,
        //            then push the coordinate (r+1,c) onto the stack and update
        //            maze[r+1][c] to indicate the algorithm has encountered it.
  
        if (maze[tempr + 1][tempc] == '.')
        {
            coordstack.push(Coord(tempr + 1, tempc));
            maze[tempr + 1][tempc] = 'X';
        }

        //        If you can move WEST and haven't encountered that cell yet,
        //            then push the coordinate (r,c-1) onto the stack and update
        //            maze[r][c-1] to indicate the algorithm has encountered it.

        if (maze[tempr][tempc - 1] == '.')
        {
            coordstack.push(Coord(tempr, tempc - 1));
            maze[tempr][tempc - 1] = 'X';
        }

        //        If you can move NORTH and haven't encountered that cell yet,
        //            then push the coordinate (r-1,c) onto the stack and update
        //            maze[r-1][c] to indicate the algorithm has encountered it.
        if (maze[tempr - 1][tempc] == '.')
        {
            coordstack.push(Coord(tempr - 1, tempc));
            maze[tempr - 1][tempc] = 'X';
        }

    }

    return false;
}
      // Return true if there is a path from (sr,sc) to (er,ec)
      // through the maze; return false otherwise


