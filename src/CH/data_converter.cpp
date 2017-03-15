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

    std::map<int64_t, uint32_t> waysFromNode;
    //Dodanie informacji o tym czy z noda sa drogi
    for(auto ways_together : document.ways())
    {
       auto ways_splitted = ways_together.second.split_me();
       for(auto& way: ways_splitted)
       {
           Endpoints endpoints = getEntpoints(way);
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
    }

    uint32_t IDWithRoads = 0;
    uint32_t IDWithoutRoads = waysFromNode.size();
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
    // "Reorder of osm2pgr nodes
    osm2pgrNodes.clear();
    for(auto& node : nodes)
    {
        osm2pgrNodes.push_back(document.nodes().at(IDconverterBack[node.id]));
    }

    nodesWithRoads = std::vector<Node>(nodes.begin(), nodes.begin() + waysFromNode.size());
    std::vector<uint32_t> order(waysFromNode.size());
    order_with_num_of_roads(&nodesWithRoads, &order);
//    simple_order(&nodesWithRoads);

    //Add edges in my format
    edgesTable.resize(waysFromNode.size());

    for(size_t j = 0; j < edgesTable.size(); ++j)
    {
        edgesTable[j].resize(waysFromNode.size(), UINT_MAX);
    }
    for(auto ways_together : document.ways())
    {
       auto ways_splitted = ways_together.second.split_me();
       for(auto& way: ways_splitted)
       {
           Endpoints endpoints = getEntpoints(way);
           assert(IDconverter.at(endpoints.start.osm_id()) < waysFromNode.size());
           assert(IDconverter.at(endpoints.end.osm_id()) < waysFromNode.size());
           edgesTable[IDconverter.at(endpoints.start.osm_id())]
                   [IDconverter.at(endpoints.end.osm_id())] = getWayCost(way);

           edgesTable[IDconverter.at(endpoints.end.osm_id())]
                   [IDconverter.at(endpoints.start.osm_id())]= getWayCost(way);
        }
    }
    nextWayID = (document.ways().rbegin()->first)+1;

    //Add shortcuts table
    shortcutsTable.resize(waysFromNode.size());
    for(size_t j = 0; j < shortcutsTable.size(); ++j)
    {
        shortcutsTable[j].resize(waysFromNode.size());
    }

    contract(edgesTable, nodesWithRoads, shortcutsTable, order);

    //Add new roads;
    std::vector<osm2pgr::Way> newWays = createNewWays(document);

    std::cout << " TYLE SKROTOW POWSTALO " << newWays.size() << std::endl;

    std::map<int64_t, Way> copyOfWays(document.ways());
    document.clear();

    for(auto ways_together : copyOfWays)
    {
       auto ways_splitted = ways_together.second.split_me();
       for(auto& way: ways_splitted)
       {
//        document.ways().erase(ways_together.second);
        Way newWay(ways_together.second);
        newWay.nodeRefs().clear();
        newWay.add_node(way.front());
        newWay.add_node(way.back());
        assert(IDconverter.find(newWay.nodeRefs().back()->osm_id()) != IDconverter.end());
        assert(IDconverter.find(newWay.nodeRefs().front()->osm_id()) != IDconverter.end());
        const unsigned int aID = IDconverter.at(newWay.nodeRefs().back()->osm_id());
        const unsigned int bID = IDconverter.at(newWay.nodeRefs().front()->osm_id());
        assert(aID < nodesWithRoads.size());
        assert(bID < nodesWithRoads.size());
        //TODO check aID == bID
        assert( nodesWithRoads[aID].order !=  nodesWithRoads[bID].order || aID == bID);
        newWay.increasingOrder = nodesWithRoads[aID].order > nodesWithRoads[bID].order;
        newWay.shortcut = false;
        newWay.setID(nextWayID++);
        document.AddWay(newWay);
        }
    }
    for(auto& way : newWays)
    {
        assert(document.ways().find(way.osm_id()) == document.ways().end());
        document.AddWay(way);
    }
}

double DataConverter::getWayCost(const std::vector<osm2pgr::Node*> &nodes) const
{
    return Way::length(nodes);
}

Endpoints DataConverter::getEntpoints(const std::vector<osm2pgr::Node*> &nodes) const
{
    return {*(nodes.front()), *(nodes.back())};
}

std::vector<Way> DataConverter::createNewWays(const OSMDocument &document)
{
    std::vector<Way> newWays;
    for(uint32_t n = 0; n < nodesWithRoads.size(); ++n )
        for(uint32_t m = n+1; m < nodesWithRoads.size(); ++m ){
            if(shortcutsTable[n][m].size()){
                Way newWay;
                for(uint32_t i = 0; i < shortcutsTable[n][m].size(); ++i){
                    newWay.add_node(&(osm2pgrNodes[shortcutsTable[n][m][i]]));
                }
                newWay.setID(nextWayID++);
                newWay.maxspeed_backward(51);
                newWay.maxspeed_forward(51);
                assert(nodesWithRoads[n].order != nodesWithRoads[m].order);
                newWay.increasingOrder = nodesWithRoads[m].order > nodesWithRoads[n].order;
                newWay.shortcut = true;
                //TODO Hack tu jest - tag pierwszy z brzegu
                newWay.tag_config((document.ways().begin()->second).tag_config());
                newWays.push_back(newWay);
            }
        }
    return newWays;
}
