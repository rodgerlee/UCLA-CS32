//
//  CarMap.h
//  Homework1
//
//  Created by Rodger Lee on 1/13/20.
//  Copyright © 2020 Rodger Lee. All rights reserved.
//

#ifndef CarMap_h
#define CarMap_h
#include <string>

#include "Map.h"
// keytype and value type are std::string and double respectively in Map.h

  class CarMap
  {
    public:
      CarMap();       // Create an empty car map.

      bool addCar(KeyType license);
        // If a car with the given license plate is not currently in the map,
        // and there is room in the map, add an entry for that car recording
        // that it has been driven 0 miles, and return true.  Otherwise,
        // make no change to the map and return false.

      double miles(KeyType license) const;
        // If a car with the given license plate is in the map, return how
        // many miles it has been driven; otherwise, return -1.

      bool drive(KeyType license, ValueType distance);
        // If no car with the given license plate is in the map or if
        // distance is negative, make no change to the map and return
        // false.  Otherwise, increase by the distance parameter the number
        // of miles the indicated car has been driven and return true.

      int fleetSize() const;  // Return the number of cars in the CarMap.

      void print() const;
        // Write to cout one line for every car in the map.  Each line
        // consists of the car's license plate, followed by one space,
        // followed by the number of miles that car has been driven.  Write
        // no other text.  The lines need not be in any particular order.

    private:
      
      Map carmap;
      
      // Some of your code goes here.
  };
#endif /* CarMap_h */
