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
    //Convert nodes to my format
    size_t numberOfNodes = document.nodes().size();
    nodes.resize(numberOfNodes);
    unsigned int i = 0;
    for(auto& node : document.nodes())
    {
        nodes[i].id = i;
        IDconverter[node.second.osm_id()] = nodes[i].id;
        i++;
    }

    //Add edges in my format
    edgesTable.resize(nodes.size());
    for(size_t j = 0; j < edgesTable.size(); ++j)
    {
        edgesTable[j].resize(nodes.size(), UINT_MAX);
    }
    for(auto& way : document.ways())
    {
       Endpoints endpoints = getEntpoints(way.second);
       edgesTable[IDconverter.at(endpoints.start.osm_id())]
               [IDconverter.at(endpoints.end.osm_id())] = getWayCost(way.second);

       edgesTable[IDconverter.at(endpoints.end.osm_id())]
               [IDconverter.at(endpoints.start.osm_id())]= getWayCost(way.second);
    }

    //Add shortcuts table
    shortcutsTable.resize(nodes.size());
    for(size_t j = 0; j < shortcutsTable.size(); ++j)
    {
        shortcutsTable[j].resize(nodes.size());
    }

    contract(edgesTable, nodes, shortcutsTable);

    size_t shorctutsSize = 0;
    for(unsigned n = 0; n < nodes.size(); ++n )
        for(unsigned m = 0; m < nodes.size(); ++m )
            shorctutsSize += shortcutsTable[n][m].size();
    std::cout << " TYLE SKROTOW POWSTALO " << shorctutsSize << std::endl;
}

double DataConverter::getWayCost(const Way &way) const
{
    return way.length();
}

Endpoints DataConverter::getEntpoints(const Way &way) const
{
    const std::vector<osm2pgr::Node*> nodes = way.nodeRefs();
    return {*(nodes.front()), *(nodes.back())};
}
