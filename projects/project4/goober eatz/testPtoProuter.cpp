////
////  testPtoProuter.cpp
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
//int main()  //test streetmap.cpp
//{
//    StreetMap* s = new StreetMap;
//    s->load("/Users/rodgerlee/homework/CS32/cs32 - rodger/projects/project4/goober eats/goober eats/mapdata.txt");
//
//
//    GeoCoord start = GeoCoord("34.0625329", "-118.4470263");
//    GeoCoord end = GeoCoord("34.0712323", "-118.4505969");
////    vector<StreetSegment> seg;
//    list<StreetSegment> seg;
//    double totalDistanceTravelled;
//    PointToPointRouter currentroute(s);
//    currentroute.generatePointToPointRoute(start, end, seg, totalDistanceTravelled);
////    34.0411322 -118.4984104
//
////    s->getSegmentsThatStartWith(start, seg);
//    list<StreetSegment>::iterator it = seg.begin();
//    while(it != seg.end())
//    {
//
//        cout << (*it).name << " " << (*it).start.latitude << " " << (*it).start.longitude << " " << (*it).end.latitude << " " << (*it).end.longitude << endl;
//        cout << endl;
//        it++;
//    }
//    cout << totalDistanceTravelled << " mi" << endl;
//}
