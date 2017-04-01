#ifndef CONTRACTING_HPP
#define CONTRACTING_HPP

#include "../ch_types.h"
#include "../shortest_path_algorithms/dijkstra_utils.hpp"

namespace RouterCH
{

void contract(EdgesTable& edgesTable, Nodes *nodes, ShorctutsTable& shortcutsTable,
              Order& order, NeighboursTable& neighboursTable);

}
#endif // CONTRACTING_HPP
