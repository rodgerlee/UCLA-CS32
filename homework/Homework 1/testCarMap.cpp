#include <iostream>
#include "CarMap.h"
#include <cassert>
using namespace std;

void test()
{
    CarMap m;
    
    assert(m.addCar("car"));
    assert(!m.addCar("car"));
    assert(m.addCar("truck"));
    assert(m.addCar("bike"));
    assert(m.fleetSize() == 3);
    m.print();
    assert(!m.drive("car", -1));
    assert(m.drive("car",5));
    assert(m.miles("car") == 5);
    assert(m.drive("car",5.5));
    assert(m.miles("car") == 10.5);
    m.print();
}

int main()
{
    test();
    cout << "Passed all tests" << endl;
    return 1;
}
