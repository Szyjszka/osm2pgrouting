#ifndef DISTANCEMANAGER_HPP
#define DISTANCEMANAGER_HPP

#include "ch_types.h"

namespace RouterCH
{
class DistanceManager
{
public:
    enum class DistanceType{
        GeoEuclides,
        GeoTaxi,
        Road
    };

    DistanceManager(const RouterCH::Nodes &nodes, const EdgesTable &edgesTable,
                    const NeighboursTable& neighboursTable);
    void removeNodeFromOwning(const uint32_t node, const uint32_t staringOrder);
    uint32_t getNumbersOfOwnedNodes(const uint32_t node) const;

private:
    struct DistanceNode{
        DistanceNode(uint32_t id_, double distance_)
            : id(id_)
            , distance(distance_)
        {
        }

        uint32_t id;
        double distance;
    };

    void setOwner(const uint32_t node, const uint32_t owner, const double distance);
    DistanceNode getClosestNode(const uint32_t node, const uint32_t startingOrder) const;
    double getDistanceToNode(const uint32_t nodeA, const uint32_t nodeB) const;

    Route closestDijkstra(const uint32_t start,  const uint32_t startingOrder);

    std::vector<std::vector<uint32_t>> closestNodes;
    std::vector<DistanceNode> owners;
    const Nodes& nodes;
    const EdgesTable &edgesTable;
    const NeighboursTable& neighboursTable;
};

}
#endif // DISTANCEMANAGER_HPP
