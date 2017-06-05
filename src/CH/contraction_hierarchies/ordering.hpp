#ifndef ORDERING_HPP
#define ORDERING_HPP

#include "../ch_types.h"
#include "../shortest_path_algorithms/dijkstra_utils.hpp"
#include "../distancemanager.hpp"

namespace RouterCH
{
    enum class OrderCriterium
    {
        Ways,
        Random,
        Shortcuts,
        Ways_Plus_Shortcuts,
        EdgeDifference,
        ContractedNeighbours,
        VoronoiRegion,
        TimeOfContraction,
        Geo,
        SearchSpace,
        CostOfQuery,
        MyAlgorithm,
        VoronaiCombination,
        GeiserbergAlgorithm,
    };

    OrderCriterium getOrderCriteriumFromString(const std::string& string);

    void simple_order(Nodes* nodes, Order* order);
    void orderNodes(OrderCriterium criterium, Nodes& nodes, Order& order, EdgesTable &edgesTable, const uint32_t start,
               ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable, const OrderParameters& orderParameters,
               const uint32_t maxHop, const uint32_t maxSettledNodes, const DistanceManager& distanceManager);


    void orderNodes(OrderCriterium criterium, Nodes& nodes, OrderQue& orderQue, EdgesTable &edgesTable,
               ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable, const OrderParameters& orderParameters,
                    const uint32_t maxHop, const uint32_t maxSettledNodes, const DistanceManager& distanceManager);

    void updateNeighbours(OrderCriterium criterium, Nodes& nodes, Order& order, EdgesTable &edgesTable, const uint32_t start,
                    ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable, const Neighbours& nodesThatChanged, const OrderParameters& orderParameters,
                          const uint32_t maxHop, const uint32_t maxSettledNodes, const DistanceManager& distanceManager);

    int32_t getOrderPoints(OrderCriterium criterium, EdgesTable& edgesTable, const Node& v, Nodes &nodes,
                            ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable, const uint32_t actualIter, const OrderParameters& orderParameters,
                           const uint32_t maxHop, const uint32_t maxSettledNodes, const DistanceManager& distanceManager);
}
#endif // ORDERING_HPP
