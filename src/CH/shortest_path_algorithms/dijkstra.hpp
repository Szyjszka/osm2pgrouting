#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include "../ch_types.h"

namespace RouterCH
{

Route dijkstra(const EdgesTable& edgesTable, const uint32_t start,
              const uint32_t end, const Nodes& nodes);
}
#endif // DIJKSTRA_HPP
