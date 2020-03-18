//
//  CarMap.cpp
//  Homework1
//
//  Created by Rodger Lee on 1/13/20.
//  Copyright © 2020 Rodger Lee. All rights reserved.
//

#include <stdio.h>

#include "Map.h"
#include "CarMap.h"
#include <iostream>
#include <string>

using namespace std;

CarMap::CarMap()
{
}// Create an empty car map.

bool CarMap::addCar(KeyType license)
{
    return carmap.insert(license, 0);

}
  // If a car with the given license plate is not currently in the map,
  // and there is room in the map, add an entry for that car recording
  // that it has been driven 0 miles, and return true.  Otherwise,
  // make no change to the map and return false.

double CarMap::miles(KeyType license) const
{
    ValueType miles;
    if (carmap.get(license, miles))
        return miles;
    return -1;
    
}
// If a car with the given license plate is in the map, return how
  // many miles it has been driven; otherwise, return -1.

bool CarMap::drive(KeyType license, ValueType distance)
{
    
    if (distance < 0 || !carmap.contains(license) )
        return false;
    ValueType currentmileage;
    carmap.get(license, currentmileage);
    return carmap.update(license, currentmileage + distance);

        
}
  // If no car with the given license plate is in the map or if
  // distance is negative, make no change to the map and return
  // false.  Otherwise, increase by the distance parameter the number
  // of miles the indicated car has been driven and return true.

int CarMap::fleetSize() const
{
    return carmap.size();
}
// Return the number of cars in the CarMap.

void CarMap::print() const
{
    int cars = fleetSize();
    KeyType m_license;
    ValueType m_distance;
    for (int i = 0; i < cars; i++)
    {
        carmap.get(i, m_license, m_distance);
        cout << m_license << " " << m_distance << '\n';
    }
}

  // Write to cout one line for every car in the map.  Each line
  // consists of the car's license plate, followed by one space,
  // followed by the number of miles that car has been driven.  Write
  // no other text.  The lines need not be in any particular order.

