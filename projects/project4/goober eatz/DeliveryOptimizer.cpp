#include "provided.h"
#include "ExpandableHashMap.h"
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;

struct cmp{
    bool operator()(const DeliveryRequest &a, const DeliveryRequest &b)
    {
        return a.item < b.item;
    }
};

class DeliveryOptimizerImpl
{
public:
    DeliveryOptimizerImpl(const StreetMap* sm);
    ~DeliveryOptimizerImpl();
    void optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const;
private:
    const StreetMap* m_sm;
    double calculateCrowDistance(const vector<DeliveryRequest> requests, const GeoCoord depot) const;
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
: m_sm(sm)
{
}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl()
{
}


void DeliveryOptimizerImpl::optimizeDeliveryOrder(
    const GeoCoord& depot,
    vector<DeliveryRequest>& deliveries,
    double& oldCrowDistance,
    double& newCrowDistance) const
{
//     initialize everything
    oldCrowDistance = 0; newCrowDistance = 0;

    oldCrowDistance = calculateCrowDistance(deliveries, depot);
    
    ExpandableHashMap<double, vector<DeliveryRequest>> delivery_map;
    //use min heap
    priority_queue<double, vector<double>, greater<double> > min_crowdistance;
    
    vector<DeliveryRequest>::iterator start = deliveries.begin();
    vector<DeliveryRequest>::iterator end = deliveries.end();
    
    // calculate all permutations;
    do {
        double tempcrowdistance = calculateCrowDistance(deliveries, depot);
        min_crowdistance.push(tempcrowdistance); //O(logN!)
        delivery_map.associate(tempcrowdistance, deliveries);
    }
    while (next_permutation(start, end, cmp()));  //O(N!)
    
    newCrowDistance = min_crowdistance.top();

    deliveries = *delivery_map.find(newCrowDistance);
    // calculate crow distance
}


double DeliveryOptimizerImpl::calculateCrowDistance(const vector<DeliveryRequest> requests, const GeoCoord depot) const
{
    double crowDistance = 0;
    // calculate "old crow distance"
    for (int i = 0; i < requests.size()-1; i++)
    {
        if (i == 0) // the first delivery from depot to 1st delivery
            crowDistance += distanceEarthMiles(depot, requests[i].location);
        else // all the other deliveries
            crowDistance += distanceEarthMiles(requests[i-1].location, requests[i].location);
    }
    crowDistance += distanceEarthMiles(requests.back().location, depot);

    return crowDistance;
}



//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm)
{
    m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer()
{
    delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const
{
    return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}
