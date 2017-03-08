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
    for(auto& node : document.nodes())
    {
        osm2pgrNodes.push_back(node.second);
    }

    std::map<int64_t, size_t> waysFromNode;
    //Dodanie informacji o tym czy z noda sa drogi
    for(auto& way : document.ways())
    {
       Endpoints endpoints = getEntpoints(way.second);
       if(waysFromNode.find(endpoints.start.osm_id()) == waysFromNode.end())
       {
           waysFromNode[endpoints.start.osm_id()] = 1;
       }
       else
       {
           ++waysFromNode[endpoints.start.osm_id()];
       }
       if(waysFromNode.find(endpoints.end.osm_id()) == waysFromNode.end())
       {
           waysFromNode[endpoints.end.osm_id()] = 1;
       }
       else
       {
           ++waysFromNode[endpoints.end.osm_id()];
       }
    }

    size_t IDWithRoads = 0;
    size_t IDWithoutRoads = waysFromNode.size();
    for(auto& node : osm2pgrNodes)
    {
        if(waysFromNode.find(node.osm_id()) != waysFromNode.end())
        {
            assert(IDWithRoads < waysFromNode.size());
            nodes[IDWithRoads].id = IDWithRoads;
            nodes[IDWithRoads].numOfWays = waysFromNode[node.osm_id()];
            IDconverter[node.osm_id()] = nodes[IDWithRoads].id;
            IDconverterBack[nodes[IDWithRoads].id] = node.osm_id();
            ++IDWithRoads;
        }
        else
        {

            assert(IDWithoutRoads < osm2pgrNodes.size());
            nodes[IDWithoutRoads].id = IDWithoutRoads;
            nodes[IDWithRoads].numOfWays = 0;
            IDconverter[node.osm_id()] = nodes[IDWithoutRoads].id;
            IDconverterBack[nodes[IDWithoutRoads].id] = node.osm_id();
            ++IDWithoutRoads;
        }
    }
    //Add edges in my format
    edgesTable.resize(waysFromNode.size());

    for(size_t j = 0; j < edgesTable.size(); ++j)
    {
        edgesTable[j].resize(waysFromNode.size(), UINT_MAX);
    }
    for(auto& way : document.ways())
    {
       Endpoints endpoints = getEntpoints(way.second);
       assert(IDconverter.at(endpoints.start.osm_id()) < waysFromNode.size());
       assert(IDconverter.at(endpoints.end.osm_id()) < waysFromNode.size());
       edgesTable[IDconverter.at(endpoints.start.osm_id())]
               [IDconverter.at(endpoints.end.osm_id())] = getWayCost(way.second);

       edgesTable[IDconverter.at(endpoints.end.osm_id())]
               [IDconverter.at(endpoints.start.osm_id())]= getWayCost(way.second);
    }
    nextWayID = (document.ways().rbegin()->first)+1;

    //Add shortcuts table
    shortcutsTable.resize(waysFromNode.size());
    for(size_t j = 0; j < shortcutsTable.size(); ++j)
    {
        shortcutsTable[j].resize(waysFromNode.size());
    }

    simple_order(&nodes);
    nodesWithRoads = std::vector<Node>(nodes.begin(), nodes.begin() + waysFromNode.size());
//    order_with_num_of_roads(&nodesWithRoads);
    contract(edgesTable, nodesWithRoads, shortcutsTable);

    //Add new roads;
    std::vector<osm2pgr::Way> newWays = createNewWays(document);

    std::cout << " TYLE SKROTOW POWSTALO " << newWays.size() << std::endl;
    for(auto way : document.ways())
    {
        way.second.increasingOrder = way.second.nodeRefs().back()->order >  way.second.nodeRefs().front()->order;
        document.AddWay(way.second);
    }
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
    for(unsigned n = 0; n < nodesWithRoads.size(); ++n )
        for(unsigned m = 0; m < nodesWithRoads.size(); ++m ){
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
                newWay.increasingOrder = nodes[m].order > nodes[n].order;
                //TODO Hack tu jest - tag pierwszy z brzegu
                newWay.tag_config((document.ways().begin()->second).tag_config());
                newWays.push_back(newWay);
            }
        }
    return newWays;
}
