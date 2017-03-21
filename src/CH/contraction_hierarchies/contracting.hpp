#ifndef CONTRACTING_HPP
#define CONTRACTING_HPP

#include "../ch_types.h"
#include "../shortest_path_algorithms/dijkstra_utils.hpp"

namespace RouterCH
{

void contract(EdgesTable& edgesTable, Nodes *nodes, ShorctutsTable& shortcutsTable,
              std::vector<uint32_t> &order);

}
#endif // CONTRACTING_HPP
