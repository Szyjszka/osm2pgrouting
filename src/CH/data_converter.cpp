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
//        assert(endpoints.start.osm_id() != endpoints.end.osm_id());
        Edge edge;
        edge.way_id = way_elem.first;
        edge.cost = getWayCost(way_elem.second);
        edgesTable[endpoints.start.osm_id()][endpoints.end.osm_id()] = edge;
        edgesTable[endpoints.end.osm_id()][endpoints.start.osm_id()] = edge;
    }
    for(auto& node : document.nodes())
    {
        nodes.push_back(node.second);
    }

    std::cout << "Oryginalne drogi "  << document.ways().size() << std::endl;
//    number_of_way_order(&nodes, edgesTable, 0);
//    std::cout << "Oryginalne nody "  << document.nodes().size() << std::endl;
//    simple_order(&nodes);
    int64_t firstAvailableID = (document.ways().rbegin()->first)+1;
    contract(edgesTable, nodes, document.ways(), newWays, firstAvailableID);

//    std::cout << "Nody po kontrakcji "  << document.nodes().size() << std::endl;
    //Silne sprawdzenie czy nie ma tych samych orderów dla 0:


    for(auto& way : newWays)
    {
        Endpoints endpoints = getEntpoints(way.second);
//        assert(endpoints.start.osm_id() != endpoints.end.osm_id());
        document.AddWay(way.second);
    }
    //Zmienienie aktualnych nodow (order)
    for(auto& node : nodes)
    {
        document.AddNode(node);
    }

    int orderZero = 0;
    for(auto& node : document.nodes())
    {
        if(node.second.order == 0)
            orderZero++;
        assert(orderZero<=1);
    }

    //Dodanie informacji o malejącym/rosnącym orderze dla drogi
    for(const std::pair<int64_t, Way>& way_elem : document.ways())
    {
        Endpoints endpoints = getEntpoints(way_elem.second);
        Way way = way_elem.second;
        //TODO Czemu tworzą się drogi z jednego noda ??
        assert(endpoints.start.order != endpoints.end.order ||
               endpoints.start.osm_id() == endpoints.end.osm_id() );
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
