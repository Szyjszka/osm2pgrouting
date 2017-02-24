#include "data_converter.hpp"

#include <algorithm>

using namespace osm2pgr;


DataConverter::DataConverter(const OSMDocument &document)
{
    //Transform map to more accesible format
    std::transform(document.nodes().begin(), document.nodes().end(),
                   std::back_inserter(nodes),
                   [](std::pair<int64_t, Node> elem){ return elem.second;});

//    //Generate Edge table
//    for(const auto& n : nodes)
//    {
//        EdgesTable.push_back(Edges(size, INF));
//    }
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
