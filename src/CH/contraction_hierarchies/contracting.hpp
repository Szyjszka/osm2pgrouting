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
              OrderCriterium orderCriterium, OrderSupervisor::Strategy strategy, const OrderParameters& orderParameters);
uint32_t contractNode(EdgesTable& edgesTable, const Node& v, const Nodes &nodes,
                  ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable, bool addNewEdges, ShorctutsInfoTable &shortcutInfos,
                      const uint32_t maxHop, const uint32_t maxSettledNodes);
}
#endif // CONTRACTING_HPP
