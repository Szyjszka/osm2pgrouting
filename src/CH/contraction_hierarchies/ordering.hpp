#ifndef ORDERING_HPP
#define ORDERING_HPP

#include "../ch_types.h"

namespace RouterCH
{
    void simple_order(Nodes* nodes);
    void number_of_way_order(Nodes* nodes, const EdgesTable& edgesTable);
}
#endif // ORDERING_HPP
