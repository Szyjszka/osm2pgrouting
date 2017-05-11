#include "distancemanager.hpp"

#include <limits>
#include "ch_types.h"

using namespace RouterCH;

DistanceManager::DistanceManager(const Nodes &nodes_, const EdgesTable &edgesTable_,
                                           const NeighboursTable& neighboursTable_)
    :   nodes(nodes_)
    ,   edgesTable(edgesTable_)
    ,   neighboursTable(neighboursTable_)
{
    owners.resize(nodes.size());
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
    for(auto nodeID : closestNodes[node])
    {
        auto closestNode = getClosestNode(node, staringOrder);
        setOwner(nodeID, closestNode.id, closestNode.distance);
    }

    closestNodes[node].clear();
}

uint32_t DistanceManager::getNumbersOfOwnedNodes(const uint32_t node) const
{
    return closestNodes[node].size();
}

double DistanceManager::getDistanceToNode(const uint32_t nodeA, const uint32_t nodeB) const
{
    return 0;
}

void DistanceManager::setOwner(const uint32_t node, const uint32_t owner, const double distance)
{
    owners[node].id = owner;
    owners[node].distance = distance;

    closestNodes[owner].push_back(node);
}

DistanceManager::DistanceNode DistanceManager::getClosestNode( const uint32_t node, const uint32_t staringOrder) const
{

}
