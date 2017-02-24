#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include "../ch_types.h"

namespace RouterCH
{

Route dijkstra(const EdgesTable& edgesTable, const unsigned int start,
              const unsigned int end, const Nodes& nodes);

EdgesTable makeEdgesTable(const Graph& graph, const unsigned int size);
}
#endif // DIJKSTRA_HPP
