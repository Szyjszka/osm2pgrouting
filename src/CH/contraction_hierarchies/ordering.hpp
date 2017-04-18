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
        EdgeDifference,
    };

    void simple_order(Nodes* nodes, Order* order);
    void orderNodes(OrderCriterium criterium, Nodes& nodes, Order& order, EdgesTable &edgesTable, const uint32_t start,
               ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable);

    void orderNodes(OrderCriterium criterium, Nodes& nodes, OrderQue& orderQue, EdgesTable &edgesTable,
               ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable);

    void updateNeighbours(OrderCriterium criterium, Nodes& nodes, Order& order, EdgesTable &edgesTable, const uint32_t start,
                    ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable, const Neighbours& nodesThatChanged);
}
#endif // ORDERING_HPP
