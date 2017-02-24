#include "data_converter.hpp"

#include <algorithm>

using namespace osm2pgr;


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
        edge.end_id = endpoints.end.osm_id();
        edge.way_id = way_elem.first;
        edge.cost = getWayCost(way_elem.second);
        edgesTable[endpoints.start.osm_id()].push_back(edge);
    }
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
