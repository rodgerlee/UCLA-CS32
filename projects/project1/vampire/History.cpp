//
//  History.cpp
//  vampire2
//
//  Created by Rodger Lee on 1/9/20.
//  Copyright © 2020 Rodger Lee. All rights reserved.
//

#include <iostream>
using namespace std;
#include "History.h"
#include "globals.h"

History::History(int nRows, int nCols)
:row(nRows), col(nCols)
{
    for (int r = 0; r < row; r++)
        for (int c = 0; c < col; c++)
            tempgrid[r][c] = 0;
}

bool History::record(int r, int c)
{
    if (r <= 0 || c <= 0 || r > row || c > col)
        return false;
    tempgrid[r-1][c-1]++;
    return true;
}

void History::display() const
{
    clearScreen();
    for (int r = 0; r < row; r++)
    {
        for (int c = 0; c < col; c++)
        {
            char vampirelag = '.';
            int n = tempgrid[r][c];
            if (n >= 26)
                vampirelag = 'Z';
            else if (n > 0)
                vampirelag = 'A' + n-1;
            cout << vampirelag;
        }
        cout << endl;
    }
    cout << endl;
}
