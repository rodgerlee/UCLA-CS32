//
//  testDeliveryPlanner.cpp
//  goober eats
//
//  Created by Rodger Lee on 3/10/20.
//  Copyright © 2020 Rodger Lee. All rights reserved.
//

#include "provided.h"
#include "ExpandableHashMap.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

bool loadDeliveryRequests(string deliveriesFile, GeoCoord& depot, vector<DeliveryRequest>& v);
bool parseDelivery(string line, string& lat, string& lon, string& item);


int main()  //test streetmap.cpp
{
    StreetMap* s = new StreetMap;
    s->load("/Users/rodgerlee/homework/CS32/cs32 - rodger/projects/project4/goober eats/goober eats/mapdata.txt");
    
    DeliveryPlanner deliveryplan(s);
    
    
    GeoCoord depot;
    vector<DeliveryRequest> deliveries;

    if (!loadDeliveryRequests("/Users/rodgerlee/homework/CS32/cs32 - rodger/projects/project4/goober eats/goober eats/deliveries.txt", depot, deliveries))
    {
        cout << "Unable to load delivery request file " << "/Users/rodgerlee/homework/CS32/cs32 - rodger/projects/project4/goober eats/goober eats/deliveries.txt" << endl;
        return 1;
    }
    
    vector<DeliveryCommand> dcs;
    double totalMiles;
    DeliveryResult result = deliveryplan.generateDeliveryPlan(depot, deliveries, dcs, totalMiles);
    if (result == BAD_COORD)
    {
        cout << "One or more depot or delivery coordinates are invalid." << endl;
        return 1;
    }
    if (result == NO_ROUTE)
    {
        cout << "No route can be found to deliver all items." << endl;
        return 1;
    }
    cout << "Starting at the depot...\n";
    for (const auto& dc : dcs)
        cout << dc.description() << endl;
    cout << "You are back at the depot and your deliveries are done!\n";
    cout.setf(ios::fixed);
    cout.precision(2);
    cout << totalMiles << " miles travelled for all deliveries." << endl;
}

bool loadDeliveryRequests(string deliveriesFile, GeoCoord& depot, vector<DeliveryRequest>& v)
{
    ifstream inf(deliveriesFile);
    if (!inf)
        return false;
    string lat;
    string lon;
    inf >> lat >> lon;
    inf.ignore(10000, '\n');
    depot = GeoCoord(lat, lon);
    string line;
    while (getline(inf, line))
    {
        string item;
        if (parseDelivery(line, lat, lon, item))
            v.push_back(DeliveryRequest(item, GeoCoord(lat, lon)));
    }
    return true;
}

bool parseDelivery(string line, string& lat, string& lon, string& item)
{
    const size_t colon = line.find(':');
    if (colon == string::npos)
    {
        cout << "Missing colon in deliveries file line: " << line << endl;
        return false;
    }
    istringstream iss(line.substr(0, colon));
    if (!(iss >> lat >> lon))
    {
        cout << "Bad format in deliveries file line: " << line << endl;
        return false;
    }
    item = line.substr(colon + 1);
    if (item.empty())
    {
        cout << "Missing item in deliveries file line: " << line << endl;
        return false;
    }
    return true;
}
