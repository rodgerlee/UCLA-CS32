#include "provided.h"
#include <string>
#include <vector>
#include <functional>
using namespace std;

//for file I/O
#include <iostream>
#include <fstream>

//for the Hash Table
#include "ExpandableHashMap.h"

//////////////////
///hasher implementation///
//////////////////

unsigned int hasher(const GeoCoord& g)
{
    return std::hash<string>()(g.latitudeText + g.longitudeText);
}

unsigned int hasher(const std::string& s)
{
    return std::hash<std::string>()(s);
}

unsigned int hasher(const int& i)
{
    return std::hash<int>()(i);
}

unsigned int hasher(const double& d)
{
    return std::hash<double>()(d);
}

//////////////////
///StreetMapImpl implementation///
//////////////////
class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
    
private:
    //helper function
    StreetSegment Reverse(StreetSegment seg);
    ExpandableHashMap<GeoCoord, vector<StreetSegment>> m_mapassociations;
    vector<StreetSegment> streetsegments;
    bool isgeocoord(string currentstring);
    
};

StreetMapImpl::StreetMapImpl()
{}

StreetMapImpl::~StreetMapImpl()
{}

bool StreetMapImpl::load(string mapFile)
{
    ifstream infile(mapFile);    // infile is a name of our choosing
    
    if ( ! infile )                // Did opening the file fail?
    {
        return false;
    }
    
    string currentstring;
    string streetname;
    // getline returns infile; the while tests its success/failure state
    while (getline(infile, currentstring))
    {
        //txt.file will always start with a street name, followed by the number of segments on the next line
        if (!isgeocoord(currentstring))
            streetname = currentstring;

        getline(infile, currentstring);
        int count = stoi(currentstring);
        
        StreetSegment revseg, currseg;
        GeoCoord startingGeoCoord, endingGeoCoord;
        vector<StreetSegment> seg;
        
        //read the geocoordinate lines
        for (int i = 0; i < count; i++)
        {
            getline(infile, currentstring);
            string slat = "", slon = "", elat = "", elon = "";
            int space = 0;

            
            for (int j = 0; j < currentstring.size(); j++)
            {
                if (currentstring[j] == ' ')
                {
                    space++;
                    if (space % 2 == 0)
                        startingGeoCoord = GeoCoord(slat, slon);
            
                    continue;
                }
                        
                if (space % 2 == 0)
                {
                    if (space < 2)
                        slat += currentstring[j];
                    else
                        elat += currentstring[j];
                }
                if (space % 2 == 1)
                {
                    if (space < 2)
                        slon += currentstring[j];
                    else
                        elon += currentstring[j];
                }
                    
            }
            endingGeoCoord = GeoCoord(elat, elon);
            
            //find geocoord in hashmap, nullptr = is not yet associated
            vector<StreetSegment>* startptr = m_mapassociations.find(startingGeoCoord);
            vector<StreetSegment>* endptr = m_mapassociations.find(endingGeoCoord);
            
            currseg = StreetSegment(startingGeoCoord, endingGeoCoord, streetname);
            revseg = Reverse(currseg);
            
            if(startptr == nullptr)
            {
                seg.push_back(currseg);
                
                m_mapassociations.associate(startingGeoCoord, seg);
                seg.clear();
            }
            
            //if geocoord already exists and contains a corresponding value, push onto that vector.
            else
            {
                (*startptr).push_back(currseg);
            }
            
            if (endptr == nullptr)
            {
                seg.push_back(revseg);
                
                m_mapassociations.associate(endingGeoCoord, seg);
                seg.clear();
            }
            
            else
            {
                (*endptr).push_back(revseg);
            }

        } //done loading line and associating geocoords
        
    } //
    
    return true;
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{

    if (m_mapassociations.find(gc))  //O(1)
    {
        segs = *m_mapassociations.find(gc);
        return true;
    }
    
    else
        return false;
        
    
//    return false;  // Delete this line and implement this function correctly
}

bool StreetMapImpl::isgeocoord(string currentstring)
{
    if (currentstring[2] != '.')
        return false;
    else
        return true;
}

StreetSegment StreetMapImpl::Reverse(StreetSegment seg)
{
    StreetSegment reverseseg = StreetSegment(seg.end, seg.start, seg.name);
    return reverseseg;
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
    delete m_impl;
}

bool StreetMap::load(string mapFile)
{
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
   return m_impl->getSegmentsThatStartWith(gc, segs);
}
