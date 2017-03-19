#include "data_converter.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include "contraction_hierarchies/contracting.hpp"
#include "contraction_hierarchies/ordering.hpp"
#include "shortest_path_algorithms/dijkstra.hpp"
#include "shortest_path_algorithms/modified_bidirectional_dijkstra.hpp"

using namespace osm2pgr;
using namespace RouterCH;


static const int64_t id_table[] = { 31257922};

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
    splittedWays = createSplittedWays(document);
    for(auto& way: splittedWays)
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

    uint32_t IDWithRoads = 0;
    uint32_t IDWithoutRoads = static_cast<uint32_t>(waysFromNode.size());
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

    //Add edges in my format
    edgesTable.resize(waysFromNode.size());

    for(size_t j = 0; j < edgesTable.size(); ++j)
    {
        edgesTable[j].resize(waysFromNode.size(), std::numeric_limits<double>::max());
    }
    for(auto& way: splittedWays)
    {
        Endpoints endpoints = getEntpoints(way);
        assert(IDconverter.at(endpoints.start.osm_id()) < waysFromNode.size());
        assert(IDconverter.at(endpoints.end.osm_id()) < waysFromNode.size());
        edgesTable[IDconverter.at(endpoints.start.osm_id())]
                [IDconverter.at(endpoints.end.osm_id())] = getWayCost(way);

        edgesTable[IDconverter.at(endpoints.end.osm_id())]
                [IDconverter.at(endpoints.start.osm_id())]= getWayCost(way);
    }

    nodesWithRoads = std::vector<Node>(nodes.begin(), nodes.begin() + waysFromNode.size());
    std::vector<uint32_t> order(waysFromNode.size());
    simple_order(&nodesWithRoads, &order);
    order_with_number_of_shorctuts(&nodesWithRoads, &order, edgesTable, 0);

    nextWayID = (document.ways().rbegin()->first)+1;

    //Add shortcuts table
    shortcutsTable.resize(waysFromNode.size());
    for(size_t j = 0; j < shortcutsTable.size(); ++j)
    {
        shortcutsTable[j].resize(waysFromNode.size());
    }

    contract(edgesTable, &nodesWithRoads, shortcutsTable, order, IDconverterBack);
//    std::cout << " ID 11 " << IDconverter.at(2401955174) << std::endl;
//    std::cout << " ID 6 " << IDconverter.at(352671528) << std::endl;
//    for(int i = 0; i < nodesWithRoads.size(); ++i)
//    for(int j = i +1; j < nodesWithRoads.size(); ++j)
////    int i = 3;
////    int j = 4;
    {
//        std::cout << "Do sprawdzenia " << nodesWithRoads.size() - i << std::endl;
//        Route sh = dijkstra(edgesTable, i, j, nodesWithRoads);
////        assert(sh.nodes.size());
//        Route sh2 = modified_bidirectional_dijkstra(edgesTable, i, j,
//                                                    nodesWithRoads, shortcutsTable);
//        if((!sh2.nodes.size() || sh2.cost >= (sh.cost + 0.000001)) && sh.nodes.size())
//        {
//            edgesTable[i][j] = sh.cost;
//            edgesTable[j][i] = sh.cost;
//            shortcutsTable[i][j].clear();
//            shortcutsTable[j][i].clear();
//            shortcutsTable[i][j].push_back(i);
//            shortcutsTable[i][j].push_back(j);
//            shortcutsTable[j][i].push_back(j);
//            shortcutsTable[j][i].push_back(i);
//        }
//////        std::cout << "sh2 size " << sh2.nodes.size() << std::endl;
//        Route sh3 = modified_bidirectional_dijkstra(edgesTable, i, j,
//                                                    nodesWithRoads, shortcutsTable);
//        assert((sh3.nodes.size() && sh3.cost <= (sh.cost + 0.00001)) || !sh.nodes.size());
    }

    std::vector<osm2pgr::Way> newWays = createNewWays(document);

    std::cout << " TYLE SKROTOW POWSTALO " << newWays.size() << std::endl;

    std::map<int64_t, Way> copyOfWays(document.ways());
    document.clear();

    for(auto ways_together : copyOfWays)
    {
       if (ways_together.second.tag_config().key() == "" || ways_together.second.tag_config().value() == "") continue;
       bool found = true;
       for(size_t i = 0; i < sizeof(id_table)/sizeof(id_table[0]); ++i)
       {
           if(ways_together.second.osm_id() == id_table[i])
           {
               found = true;
           }
       }
       if(!found)
       {
           continue;
       }
       auto ways_splitted = ways_together.second.split_me();
       for(auto& way: ways_splitted)
       {
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
        if(aID == bID)
        {
            continue;
        }
        assert( nodesWithRoads[aID].order !=  nodesWithRoads[bID].order || aID == bID);
        newWay.increasingOrder = nodesWithRoads[aID].order > nodesWithRoads[bID].order;
        newWay.shortcut = -1;
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
                newWay.add_node(&(osm2pgrNodes[n]));
                for(uint32_t i = 0; i < shortcutsTable[n][m].size(); ++i){
                    assert(shortcutsTable[n][m][i] < nodesWithRoads.size());
                    newWay.add_node(&(osm2pgrNodes[shortcutsTable[n][m][i]]));
                }
                newWay.add_node(&(osm2pgrNodes[m]));
                newWay.setID(nextWayID++);
                newWay.maxspeed_backward(51);
                newWay.maxspeed_forward(51);
                assert(nodesWithRoads[n].order != nodesWithRoads[m].order);
                newWay.increasingOrder = nodesWithRoads[m].order > nodesWithRoads[n].order;
                newWay.shortcut = 0;
                //TODO Hack tu jest - tag pierwszy z brzegu
                newWay.tag_config((document.ways().begin()->second).tag_config());
                newWays.push_back(newWay);
            }
        }
    return newWays;
}

DataConverter::SplittedWays DataConverter::createSplittedWays(const OSMDocument &document)
{
    SplittedWays splittedWays;
    for(auto ways_together : document.ways())
    {
       if (ways_together.second.tag_config().key() == "" || ways_together.second.tag_config().value() == "") continue;
       bool found = true;
       for(unsigned int i = 0; i < sizeof(id_table)/sizeof(id_table[0]); ++i)
       {
           if(ways_together.second.osm_id() == id_table[i])
           {
               found = true;
           }
       }
       if(!found)
       {
           continue;
       }

       auto ways_splitted = ways_together.second.split_me();
       for(auto way : ways_splitted)
       {
           splittedWays.push_back(way);
       }
    }
    return splittedWays;
}
