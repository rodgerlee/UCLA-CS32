////
////  testHashMap.cpp
////  goober eats
////
////  Created by Rodger Lee on 3/9/20.
////  Copyright © 2020 Rodger Lee. All rights reserved.
////
//
//#include "provided.h"
//#include "ExpandableHashMap.h"
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <string>
//#include <vector>
//using namespace std;
//
//int main()
//{
//    ExpandableHashMap<string,double> nameToGPA(0.3);
//    // Add new items to the hashmap. Inserting the third item will cause
//    // the hashmap to increase the number of buckets (since the maximum
//    // load factor is 0.3), forcing a rehash of all items.
//    nameToGPA.associate("Carey", 3.5); // Carey has a 3.5 GPA
//    nameToGPA.associate("David", 3.99); // David beat Carey
//    nameToGPA.associate("Abe", 3.2); // Abe has a 3.2 GPA
//    double* davidsGPA = nameToGPA.find("David");
//    if (davidsGPA != nullptr)
//    *davidsGPA = 1.5; // after a re-grade of David’s exam
//    nameToGPA.associate("Carey", 4.0); // Carey deserves a 4.0
//    // replaces old 3.5 GPA
//    double* careysGPA = nameToGPA.find("Carey");
//    if (careysGPA == nullptr)
//        cout << "Carey is not in the roster!" << endl;
//    else
//    cout << "Carey’s GPA is: " << *careysGPA << endl;
//}
