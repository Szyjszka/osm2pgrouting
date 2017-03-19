#ifndef ORDERING_HPP
#define ORDERING_HPP

#include "../ch_types.h"
#include "../shortest_path_algorithms/dijkstra_utils.hpp"

namespace RouterCH
{
    void simple_order(Nodes* nodes, std::vector<unsigned int>* order);
    void order_with_number_of_shorctuts(Nodes *nodes, std::vector<unsigned int>* order, const EdgesTable &edgesTable, const int start);
    void order_with_num_of_roads(Nodes* nodes, std::vector<unsigned int>* order);
    void order_with_num_of_roads(Nodes *nodes, std::vector<unsigned int>* order, const EdgesTable &edgesTable, const int start);
}
#endif // ORDERING_HPP
