#include "provided.h"
#include <vector>
using namespace std;

class DeliveryPlannerImpl
{
public:
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    DeliveryResult generateDeliveryPlan(
        const GeoCoord& depot,
        const vector<DeliveryRequest>& deliveries,
        vector<DeliveryCommand>& commands,
        double& totalDistanceTravelled) const;
    
public:
    const StreetMap* m_streetmap;
    PointToPointRouter m_router;
    string generatedirection(string dir, double angle) const;
    void generateDirectionsMain(list<StreetSegment> route, vector<DeliveryCommand>& commands) const;
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm)
: m_router(sm), m_streetmap(sm)
{
}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{}

DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{

    DeliveryOptimizer d_opt(m_streetmap);
    double oldCrowDistance, newCrowDistance;
    vector<DeliveryRequest> temp = deliveries;
    
        //optimize delivery order
    d_opt.optimizeDeliveryOrder(depot, temp, oldCrowDistance, newCrowDistance);

//    for (int i = 0; i < temp.size(); i++)
//    {
//        cout << temp[i].item << endl;
//    }
//    cout << endl << oldCrowDistance << " vs. " << newCrowDistance << endl;


    vector<DeliveryRequest>::const_iterator it = temp.begin();
    list<StreetSegment> route;
    totalDistanceTravelled = 0;
    double distancetravelled;
    list<GeoCoord> completedrequests;
    DeliveryResult tempresult = DELIVERY_SUCCESS;
    
    //generate point to point routes
    //for each set of segments (ptp route), generate delivery commands
    while (it != temp.end())
    {
        if (it == temp.begin())
            tempresult = m_router.generatePointToPointRoute(depot, (*it).location, route, distancetravelled);
        else
            tempresult = m_router.generatePointToPointRoute(completedrequests.back(), (*it).location, route, distancetravelled);
        
        if (tempresult != DELIVERY_SUCCESS)
            return tempresult;
            
        totalDistanceTravelled += distancetravelled;
        DeliveryCommand command;
        
        generateDirectionsMain(route, commands);  //create commands to food dropoff point
        
        command.initAsDeliverCommand((*it).item);  //deliver food
        commands.push_back(command);
        completedrequests.push_back((*it).location);
        
        it++;
        route.clear();
        
    }
    list<StreetSegment> backtodepot;
    
    //create commands to return back to depot.
    m_router.generatePointToPointRoute(completedrequests.back(), depot, backtodepot, distancetravelled);
    totalDistanceTravelled += distancetravelled;
    generateDirectionsMain(backtodepot, commands);
    
    return tempresult;  // Delete this line and implement this function correctly
}

void DeliveryPlannerImpl::generateDirectionsMain(list<StreetSegment> route, vector<DeliveryCommand> &commands) const
{
    list<StreetSegment>::iterator routeIt = route.begin();
    DeliveryCommand command;
    list<StreetSegment> visitedsegments;
    
    while(routeIt != route.end())  //look through each streetsegment in the route
    {
        string streetname = routeIt->name;
        if (routeIt == route.begin())
        {
            command.initAsProceedCommand(generatedirection("proceed", angleOfLine((*routeIt))), streetname, distanceEarthMiles(routeIt->start, routeIt->end));
            commands.push_back(command);
            visitedsegments.push_back((*routeIt));
            routeIt++;
        }
        
        else
        {
            //if current segment name == previous segment name, proceed and add to distance travelled
            //proceed condition
            string previousname = commands.back().streetName();
            if (routeIt->name == previousname)
            {
                while (routeIt != route.end())
                {
                    //proceed if current streetname is equal to previous streetname
                    if (routeIt->name == streetname)
                    {
                        visitedsegments.push_back((*routeIt));
                        commands.back().increaseDistance(distanceEarthMiles(routeIt->start, routeIt->end));
                        routeIt++;
                    }
                    else
                    {
                        break;
                    }
                }
                
                if (routeIt == route.end())
                    break;
            }
            
            //turn conditions
            else
            {
                StreetSegment prev = visitedsegments.back();
                StreetSegment curr = (*routeIt);
                double angle = angleBetween2Lines(prev, curr);

                if (angle >= 1 && angle <= 359)
                {
                    command.initAsTurnCommand(generatedirection("turn", angle), streetname);
                    commands.push_back(command);
                }
                
                command.initAsProceedCommand(generatedirection("proceed", angleOfLine((*routeIt))), streetname, distanceEarthMiles(routeIt->start, routeIt->end));
                commands.push_back(command);
                visitedsegments.push_back((*routeIt));
                routeIt++;
            }
        }
    } //done looping through each segment for current delivery
    

}

string DeliveryPlannerImpl::generatedirection(string dir, double angle) const
{
    if (dir == "proceed")
    {
        if (angle >= 0 && angle < 22.5)
            return "east";
        if (angle >= 22.5 && angle < 67.5)
            return "northeast";
        if (angle >= 67.5 && angle < 112.5)
            return "north";
        if (angle >= 112.5 && angle < 157.5)
            return "northwest";
        if (angle >= 157.5 && angle < 202.5)
            return "west";
        if (angle >= 202.5 && angle < 247.5)
            return "southwest";
        if (angle >= 247.5 && angle < 292.5)
            return "south";
        if (angle >= 292.5 && angle < 337.5)
            return "southeast";
        if (angle > 337.5)
            return "east";
    }
    if (dir == "turn")
    {
        if (angle >= 1 && angle < 180)
            return "left";
        if (angle >= 180 && angle <= 359)
            return "right";
    }
    return "";
}
//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
    m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
    delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}
