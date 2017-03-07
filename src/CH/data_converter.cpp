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
        osm2pgrNodes.push_back(node.second);
        IDconverter[node.second.osm_id()] = nodes[i].id;
        IDconverterBack[nodes[i].id] = node.second.osm_id();
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
    nextWayID = (document.ways().rbegin()->first)+1;

    //Add shortcuts table
    shortcutsTable.resize(nodes.size());
    for(size_t j = 0; j < shortcutsTable.size(); ++j)
    {
        shortcutsTable[j].resize(nodes.size());
    }

    simple_order(&nodes);
    contract(edgesTable, nodes, shortcutsTable);

    //Add new roads;
    std::vector<osm2pgr::Way> newWays = createNewWays(document);

    std::cout << " TYLE SKROTOW POWSTALO " << newWays.size() << std::endl;
    for(auto& way : newWays)
    {
        document.AddWay(way);
    }
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

std::vector<Way> DataConverter::createNewWays(const OSMDocument &document)
{
    std::vector<Way> newWays;
    for(unsigned n = 0; n < nodes.size(); ++n )
        for(unsigned m = 0; m < nodes.size(); ++m ){
            if(n == m)
            {
                continue;
            }
            if(shortcutsTable[n][m].size()){
                Way newWay;
                for(unsigned i = 0; i < shortcutsTable[n][m].size(); ++i){
                    newWay.add_node(&(osm2pgrNodes[i]));
                }
                newWay.setID(nextWayID++);
                newWay.maxspeed_backward(51);
                newWay.maxspeed_forward(51);
                assert(nodes[n].order != nodes[m].order);
                newWay.increasingOrder = nodes[n].order > nodes[m].order;
                //TODO Hack tu jest - tag pierwszy z brzegu
                newWay.tag_config((document.ways().begin()->second).tag_config());
                newWays.push_back(newWay);
            }
        }
    return newWays;
}
