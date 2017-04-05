#ifndef ORDERING_HPP
#define ORDERING_HPP

#include "../ch_types.h"
#include "../shortest_path_algorithms/dijkstra_utils.hpp"

namespace RouterCH
{
    void simple_order(Nodes* nodes, Order* order);
    void order_with_number_of_shorctuts(Nodes *nodes, Order* order, EdgesTable &edgesTable, const uint32_t start, ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable);
    void order_with_num_of_roads(Nodes* nodes, Order* order);
    void order_with_num_of_roads(Nodes *nodes, Order* order, const EdgesTable &edgesTable, const uint32_t start);
}
#endif // ORDERING_HPP
