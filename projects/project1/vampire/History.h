//
//  History.h
//  vampire2
//
//  Created by Rodger Lee on 1/9/20.
//  Copyright © 2020 Rodger Lee. All rights reserved.
//

#ifndef History_h
#define History_h
#include "globals.h"


class History
{
  public:
    History(int nRows, int nCols);
    
    bool record(int r, int c);
    
    void display() const;
    
  private:
    
    int tempgrid[MAXROWS][MAXCOLS];
    int row;
    int col;
    
        
};

#endif /* History_h */
