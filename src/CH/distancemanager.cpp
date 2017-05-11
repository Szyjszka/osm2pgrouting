#include "distancemanager.hpp"

#include <limits>
#include "ch_types.h"
#include "shortest_path_algorithms/dijkstra.hpp"
#include "shortest_path_algorithms/dijkstra_utils.hpp"

using namespace RouterCH;

DistanceManager::DistanceManager(const Nodes &nodes_, const EdgesTable &edgesTable_,
                                           const NeighboursTable& neighboursTable_)
    :   nodes(nodes_)
    ,   edgesTable(edgesTable_)
    ,   neighboursTable(neighboursTable_)
{
    owners.resize(nodes.size(), DistanceNode(INF, 1e6));
    closestNodes.resize(nodes.size());

    for(auto node : nodes)
    {
        auto minDistance = std::numeric_limits<double>::max();
        auto closestNode = std::numeric_limits<uint32_t>::max();
        for(auto neighbour : neighboursTable[node.id])
        {
            if(edgesTable[neighbour].at(node.id) < minDistance)
            {
                minDistance = edgesTable[neighbour].at(node.id);
                closestNode = neighbour;
            }
        }
        assert(closestNode < std::numeric_limits<uint32_t>::max());
        owners[node.id].id = closestNode;
        owners[node.id].distance = minDistance;
        closestNodes[closestNode].push_back(node.id);
    }
}

void DistanceManager::removeNodeFromOwning(const uint32_t node, const uint32_t staringOrder)
{
    if(!closestNodes[node].size())
        return;
    for(uint32_t nodeID : closestNodes[node])
    {
            auto closestNode = getClosestNode(nodeID, staringOrder + 1);
            if(closestNode.id < INF)
            {
                assert(nodes[closestNode.id].order > staringOrder);
                setOwner(nodeID, closestNode.id, closestNode.distance);
            }
    }

    closestNodes[node].clear();
}

uint32_t DistanceManager::getNumbersOfOwnedNodes(const uint32_t node) const
{
    return closestNodes[node].size();
}

void DistanceManager::setOwner(const uint32_t node, const uint32_t owner, const double distance)
{
    owners[node].id = owner;
    owners[node].distance = distance;

    closestNodes[owner].push_back(node);
}

DistanceManager::DistanceNode DistanceManager::getClosestNode( const uint32_t node, const uint32_t startingOrder) const
{
    QSTable qsTable(nodes.size(),true);
    PathTable numberOfWaysAway(nodes.size(), 0);
    CostQue costQue;
    costQue.push(std::make_pair(0.0, node));
    CostTable costTable(nodes.size(), std::numeric_limits<double>::max());
    costTable[node] = 0;
    PDI nextElem = costQue.top();

    while(costQue.size())
    {
        do{
            nextElem = costQue.top();
            costQue.pop();
        }while(!qsTable[nextElem.second] && costQue.size());

        if(nextElem.second != node)
        {
            if(nodes[nextElem.second].order >= startingOrder)
            {
                DistanceNode n(nextElem.second, nextElem.first);
                assert(n.id != node);
                return n;
            }
        }

        qsTable[nextElem.second] = false;
        for(auto edge : edgesTable.at(nextElem.second))
        {
                if(qsTable[edge.first] && edge.second  < INF)
                {
                        if(costTable[edge.first] > costTable[nextElem.second] + edge.second &&
                                nodes[edge.first].order >= startingOrder)
                        {
                            costTable[edge.first] = costTable[nextElem.second] + edge.second;
                            if(qsTable[edge.first])
                            {
                                costQue.push(std::make_pair(costTable[edge.first], edge.first));
                            }
                        }
                }
        }
    }
    DistanceNode n(INF, INF);
    return n;
}
