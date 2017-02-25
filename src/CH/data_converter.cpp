#include "data_converter.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include "contraction_hierarchies/contracting.hpp"
#include "contraction_hierarchies/ordering.hpp"

using namespace osm2pgr;
using namespace RouterCH;


DataConverter::DataConverter(OSMDocument &document)
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

    number_of_way_order(&nodes, edgesTable, 0);

    std::cout << "Oryginalne drogi "  << document.ways().size() << std::endl;

    int64_t firstAvailableID = (document.ways().rbegin()->first)+1;
    contract(edgesTable, nodes, document.ways(), newWays, firstAvailableID);

    for(auto& way : newWays)
    {
        document.AddWay(way.second);
    }
    //Zmienienie aktualnych nodow (order)
    for(auto& node : nodes)
    {
        document.AddNode(node);
    }

    //Dodanie informacji o malejącym/rosnącym orderze dla drogi
    for(const std::pair<int64_t, Way>& way_elem : document.ways())
    {
        Endpoints endpoints = getEntpoints(way_elem.second);
        Way way = way_elem.second;
        //TODO Czemu tworzą się drogi z jednego noda ??
//        assert(endpoints.start.order != endpoints.end.order);
        way.increasingOrder = (endpoints.start.order > endpoints.end.order);
        document.AddWay(way);
    }

    std::cout << "Dodane drogi "  <<  newWays.size() << std::endl;
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
