#include "data_converter.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
#include "algorithm_time_measure.hpp"
#include "contraction_hierarchies/contracting.hpp"
#include "contraction_hierarchies/ordering.hpp"
#include "shortest_path_algorithms/dijkstra.hpp"
#include "shortest_path_algorithms/modified_bidirectional_dijkstra.hpp"

using namespace osm2pgr;
using namespace RouterCH;

DataConverter::DataConverter(OSMDocument &document, OrderCriterium orderCriterium, OrderSupervisor::Strategy strategy,
                             const std::__cxx11::string &measureFileName, const OrderParameters& orderParameters)
{
    std::ofstream measureFile(measureFileName, std::ofstream::out | std::ofstream::app);
    AlgorithmTimeMeasure atm;

    convertToInternalFormat(document);

    std::cout <<"contraction" << std::endl;

    atm.startMeasurement();

    contract(edgesTable, nodesWithRoads, shortcutsTable, neighboursTable, shortcutInfos, orderCriterium, strategy, orderParameters);

    atm.stopMeasurement();

    const uint32_t numOfShortcuts = upgradeWays(document);

    measureFile << nodesWithRoads.size() << " " << document.ways().size() - numOfShortcuts << " " << numOfShortcuts << " " << atm.getMeanTime() << std::endl;

    measureFile.close();
}

double DataConverter::getWayCost(const std::vector<const osm2pgr::Node*> &nodes) const
{
    return Way::length(nodes);
}

Endpoints DataConverter::getEntpoints(const std::vector<const osm2pgr::Node*> &nodes) const
{
    return {*(nodes.front()), *(nodes.back())};
}

Tag DataConverter::getTagForNewWays(const OSMDocument &document)
{
    osm2pgr::Tag tagForNewWays;
    for(auto& way : document.ways())
    {
        tagForNewWays = way.second.tag_config();
        if(tagForNewWays.key() != "" && tagForNewWays.value() != "")
        {
            return tagForNewWays;
        }
    }
    assert(false);
}

DataConverter::Osm2pgrWays DataConverter::createNewWays(const osm2pgr::OSMDocument& document)
{
    //These things are no longer needed
    nodes.clear();
    edgesTable.clear();
    neighboursTable.clear();

    osm2pgr::Tag tagForNewWays = getTagForNewWays(document);
    Osm2pgrWays newWays;
    //TODO by iter over shortcuts
    for(uint32_t n = 0; n < nodesWithRoads.size(); ++n )
        for(uint32_t m = n+1; m < nodesWithRoads.size(); ++m ){
            if(shortcutsTable[n].find(m) != shortcutsTable[n].end()){
                if(shortcutsTable[n].at(m).size()){
                    Way newWay;
                    newWay.add_node(&(osm2pgrNodes[n]));
                    for(uint32_t i = 0; i < shortcutsTable[n][m].size(); ++i){
                        assert(shortcutsTable[n][m][i] < nodesWithRoads.size());
                        newWay.add_node(&(osm2pgrNodes[shortcutsTable[n][m][i]]));
                    }
                    newWay.shortcutID = shortcutInfos[n][m].id;
                    newWay.shA = shortcutInfos[n][m].shA;
                    newWay.shB = shortcutInfos[n][m].shB;
                    newWay.add_node(&(osm2pgrNodes[m]));
                    newWay.setID(nextWayID++);
                    newWay.maxspeed_backward(51);
                    newWay.maxspeed_forward(51);
                    assert(nodesWithRoads[n].order != nodesWithRoads[m].order);
                    newWay.increasingOrder = nodesWithRoads[m].order > nodesWithRoads[n].order;
                    newWay.shortcut = 0;
                    newWay.tag_config(tagForNewWays);
                    newWays.push_back(newWay);
                }
            }
        }
    return newWays;
}

unsigned int DataConverter::upgradeWays(OSMDocument &document)
{

    Osm2pgrWays newWays = createNewWays(document);

    std::cout << " TYLE SKROTOW POWSTALO " << newWays.size() << std::endl;

    shortcutsTable.clear();

    std::map<int64_t, Way> copyOfWays(document.ways());
    document.clear();

    for(auto ways_together : copyOfWays)
    {
       if (ways_together.second.tag_config().key() == "" || ways_together.second.tag_config().value() == "") continue;
       auto ways_splitted = ways_together.second.split_me();
       for(auto& way: ways_splitted)
       {
        Way newWay(ways_together.second);
        newWay.nodeRefs().clear();
        newWay.add_node(way.front());
        newWay.add_node(way.back());
        assert(IDconverter.find(newWay.nodeRefs().back()->osm_id()) != IDconverter.end());
        assert(IDconverter.find(newWay.nodeRefs().front()->osm_id()) != IDconverter.end());
        const uint32_t aID = IDconverter.at(newWay.nodeRefs().back()->osm_id());
        const uint32_t bID = IDconverter.at(newWay.nodeRefs().front()->osm_id());
        assert(aID < nodesWithRoads.size());
        assert(bID < nodesWithRoads.size());
        if(aID == bID)
        {
            continue;
        }
        assert( nodesWithRoads[aID].order !=  nodesWithRoads[bID].order || aID == bID);
        newWay.increasingOrder = nodesWithRoads[aID].order > nodesWithRoads[bID].order;
        newWay.shortcut = -1;
        uint32_t a = std::max(aID, bID);
        uint32_t b = std::min(aID, bID);
        newWay.shortcutID = a*nodesWithRoads.size() + b;
        newWay.shA = -1;
        newWay.shB = -1;
        newWay.setID(nextWayID++);
        document.AddWay(newWay);
        }
    }
    for(auto& way : newWays)
    {
        assert(document.ways().find(way.osm_id()) == document.ways().end());
        document.AddWay(way);
    }

    return newWays.size();
}

void DataConverter::convertToInternalFormat(const OSMDocument &document)
{

    std::cout <<"convert to internal format" << std::endl;
    getTagForNewWays(document);

    std::cout <<"create splitted ways" << std::endl;
    SplittedWays splittedWays;
    createSplittedWays(document, splittedWays);
    NumberOfWaysFromNode numberOfWaysFromNode = getNumberOfWaysFromNode(splittedWays);
    size_t numberOfWays = numberOfWaysFromNode.size();
    nodes.resize(numberOfWays);
    groupNodesWithRoads(numberOfWaysFromNode, document);
    numberOfWaysFromNode.clear();


    std::cout <<"fill edges table" << std::endl;
    std::cout <<"Tyle wierzcholkow z drogami " << osm2pgrNodes.size()<<std::endl;
    fillEdgesTable(splittedWays, numberOfWays);

    nodesWithRoads = std::vector<Node>(nodes.begin(), nodes.begin() + numberOfWays);
//    order.resize(numberOfWaysFromNode.size());

    nextWayID = (document.ways().rbegin()->first)+1;
}

DataConverter::NumberOfWaysFromNode DataConverter::getNumberOfWaysFromNode(const DataConverter::SplittedWays &splittedWays)
{
    //TODO Here can be just neighbours table
    NumberOfWaysFromNode waysFromNode;
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
    return waysFromNode;
}

void DataConverter::createSplittedWays(const OSMDocument &document, DataConverter::SplittedWays& splittedWays)
{
    splittedWays.reserve(document.ways().size());
    for(auto ways_together : document.ways())
    {
       if (ways_together.second.tag_config().key() == "" || ways_together.second.tag_config().value() == "") continue;

       auto ways_splitted = ways_together.second.split_me();
       for(auto way : ways_splitted)
       {
           splittedWays.push_back(way);
       }
    }
}

void DataConverter::groupNodesWithRoads(const DataConverter::NumberOfWaysFromNode &numberOfWaysFromNode, const OSMDocument &document)
{
    uint32_t IDWithRoads = 0;
    nodes.resize(numberOfWaysFromNode.size());
    std::map<uint32_t, int64_t> IDconverterBack;
    for(auto& node : document.nodes())
    {
        if(numberOfWaysFromNode.find(node.first) != numberOfWaysFromNode.end())
        {
            assert(IDWithRoads < numberOfWaysFromNode.size());
            nodes[IDWithRoads].id = IDWithRoads;
            nodes[IDWithRoads].orderPoints = numberOfWaysFromNode.at(node.first);
            nodes[IDWithRoads].lat = boost::lexical_cast<double>(node.second.get_attribute("lat"));
            nodes[IDWithRoads].lon = boost::lexical_cast<double>(node.second.get_attribute("lon"));
            IDconverter[node.first] = nodes[IDWithRoads].id;
            IDconverterBack[nodes[IDWithRoads].id] = node.first;
            ++IDWithRoads;
        }
    }
    // "Reorder of osm2pgr nodes
    osm2pgrNodes.resize(IDWithRoads);
    for(size_t i = 0; i < IDWithRoads; ++i)
    {
        osm2pgrNodes[i] = document.nodes().at(IDconverterBack[nodes[i].id]);
    }
}

void DataConverter::fillEdgesTable(const DataConverter::SplittedWays &splittedWays, const size_t numberOfWays)
{
    edgesTable.resize(numberOfWays);
    shortcutsTable.resize(numberOfWays);
    neighboursTable.resize(numberOfWays, Neighbours());
    shortcutInfos.resize(numberOfWays);

    for(auto& way: splittedWays)
    {
        Endpoints endpoints = getEntpoints(way);
        uint32_t u = IDconverter.at(endpoints.start.osm_id());
        uint32_t w = IDconverter.at(endpoints.end.osm_id());
        assert(u < numberOfWays);
        assert(w < numberOfWays);
        const double wayCost = getWayCost(way);
        if(u == w || edgesTable[u].find(w) != edgesTable[u].end())
        {
            continue;
        }
        edgesTable[u][w] = wayCost;
        edgesTable[w][u] = wayCost;
        neighboursTable[u].push_back(w);
        neighboursTable[w].push_back(u);
    }
}
