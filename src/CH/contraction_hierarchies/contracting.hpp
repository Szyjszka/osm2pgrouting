#ifndef CONTRACTING_HPP
#define CONTRACTING_HPP

#include "../ch_types.h"
#include "../shortest_path_algorithms/dijkstra_utils.hpp"

namespace RouterCH
{

void contract(EdgesTable& edgesTable, Nodes *nodes, ShorctutsTable& shortcutsTable,
              std::vector<unsigned int> &order, const std::map<uint32_t, int64_t>& IDconverterBack);

}
#endif // CONTRACTING_HPP
