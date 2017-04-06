#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include "../ch_types.h"

namespace RouterCH
{

Route dijkstra(const NeighboursTable& neighboursTable, const uint32_t start,
              const uint32_t end, const Nodes& nodes, const double maxCost);
}
#endif // DIJKSTRA_HPP
