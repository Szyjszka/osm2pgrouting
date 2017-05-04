#ifndef ORDERING_HPP
#define ORDERING_HPP

#include "../ch_types.h"
#include "../shortest_path_algorithms/dijkstra_utils.hpp"

namespace RouterCH
{
    enum class OrderCriterium
    {
        Ways,
        Shortcuts,
        Ways_Plus_Shortcuts,
        EdgeDifference,
        ContractedNeighbours,
        VoronoiRegion,
        TimeOfContraction,
        Geo
    };

    OrderCriterium getOrderCriteriumFromString(const std::string& string);

    void simple_order(Nodes* nodes, Order* order);
    void orderNodes(OrderCriterium criterium, Nodes& nodes, Order& order, EdgesTable &edgesTable, const uint32_t start,
               ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable);


    void orderNodes(OrderCriterium criterium, Nodes& nodes, OrderQue& orderQue, EdgesTable &edgesTable,
               ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable);

    void updateNeighbours(OrderCriterium criterium, Nodes& nodes, Order& order, EdgesTable &edgesTable, const uint32_t start,
                    ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable, const Neighbours& nodesThatChanged);

    int32_t getOrderPoints(OrderCriterium criterium, EdgesTable& edgesTable, const Node& v, Nodes &nodes,
                            ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable, const uint32_t actualIter);
}
#endif // ORDERING_HPP
