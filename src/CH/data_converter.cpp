#include "data_converter.h"

#include <algorithm>
#include <iostream>
#include "contraction_hierarchies/contracting.hpp"
#include "contraction_hierarchies/ordering.hpp"

using namespace osm2pgr;
using namespace RouterCH;


DataConverter::DataConverter(const OSMDocument &document)
{
    std::map<int64_t, bool> pushed;
    for(const std::pair<int64_t, Way>& way_elem : document.ways())
    {
        Endpoints endpoints = getEntpoints(way_elem.second);
        Edge edge;
        edge.way_id = way_elem.first;
        edge.cost = getWayCost(way_elem.second);
        edgesTable[endpoints.start.osm_id()][endpoints.end.osm_id()] = edge;
        edgesTable[endpoints.end.osm_id()][endpoints.start.osm_id()] = edge;
        if(pushed.find(endpoints.start.osm_id()) == pushed.end())
        {
            nodes.push_back(document.nodes().at(endpoints.start.osm_id()));
        }
        pushed[endpoints.start.osm_id()] = true;
        if(pushed.find(endpoints.end.osm_id()) == pushed.end())
        {
            nodes.push_back(document.nodes().at(endpoints.end.osm_id()));
        }
        pushed[endpoints.end.osm_id()] = true;
    }

    simple_order(&nodes);
    size_t ways_size = 0;
    for(const std::pair<int64_t, Edges>& ways : edgesTable)
    {
        ways_size += ways.second.size();
    }
    std::cout << "Przed dodaniem skrótów: "  << ways_size << std::endl;
    contract(edgesTable, nodes);
    ways_size = 0;
    for(const std::pair<int64_t, Edges>& ways : edgesTable)
    {
        ways_size += ways.second.size();
    }
    std::cout << "Po: "  <<  ways_size << std::endl;
}

double DataConverter::getWayCost(const Way &way) const
{
    return way.length();
}

Endpoints DataConverter::getEntpoints(const Way &way) const
{
    const std::vector<Node*> nodes = way.nodeRefs();
    return {*(nodes.front()), *(nodes.back())};
}
