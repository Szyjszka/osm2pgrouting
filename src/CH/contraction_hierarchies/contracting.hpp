#ifndef CONTRACTING_HPP
#define CONTRACTING_HPP

#include "../ch_types.h"
#include "../shortest_path_algorithms/dijkstra_utils.hpp"
#include "ordering.hpp"
#include "ordersupervisor.hpp"

namespace RouterCH
{

void contract(EdgesTable& edgesTable, Nodes& nodes, ShorctutsTable& shortcutsTable,
              NeighboursTable& neighboursTable, ShorctutsInfoTable &shortcutInfos,
              OrderCriterium orderCriterium, OrderSupervisor::Strategy strategy);
uint32_t contractNode(EdgesTable& edgesTable, const Node& v, const Nodes &nodes,
                  ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable, bool addNewEdges, ShorctutsInfoTable &shortcutInfos);
}
#endif // CONTRACTING_HPP
