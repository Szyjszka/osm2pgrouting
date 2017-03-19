#ifndef ORDERING_HPP
#define ORDERING_HPP

#include "../ch_types.h"

namespace RouterCH
{
    void simple_order(Nodes* nodes, std::vector<unsigned int>* order);
    void order_with_num_of_roads(Nodes* nodes, std::vector<unsigned int>* order, const EdgesTable& edgesTable);
    void order_with_num_of_roads(Nodes *nodes, std::vector<unsigned int>* order, const EdgesTable &edgesTable, const int start);
}
#endif // ORDERING_HPP
