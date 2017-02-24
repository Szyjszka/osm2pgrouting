#include "data_converter.h"

#include <algorithm>
#include <iostream>
#include "contraction_hierarchies/contracting.hpp"
#include "contraction_hierarchies/ordering.hpp"

using namespace osm2pgr;
using namespace RouterCH;


DataConverter::DataConverter(const OSMDocument &document)
{
    //Transform map to more accesible format
    std::transform(document.nodes().begin(), document.nodes().end(),
                   std::back_inserter(nodes),
                   [](std::pair<int64_t, Node> elem){ return elem.second;});

    for(const std::pair<int64_t, Way>& way_elem : document.ways())
    {
        Endpoints endpoints = getEntpoints(way_elem.second);
        Edge edge;
        edge.way_id = way_elem.first;
        edge.cost = getWayCost(way_elem.second);
        edgesTable[endpoints.start.osm_id()][endpoints.end.osm_id()] = edge;
        edgesTable[endpoints.end.osm_id()][endpoints.start.osm_id()] = edge;
    }

    simple_order(&nodes);
    std::cout << "Przed dodaniem skrótów: "  << edgesTable.size() << std::endl;
    contract(edgesTable, nodes);
    std::cout << "Po: "  <<  edgesTable.size() << std::endl;
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
