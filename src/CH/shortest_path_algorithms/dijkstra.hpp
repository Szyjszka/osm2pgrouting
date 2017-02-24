#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include "../ch_types.h"

namespace RouterCH
{

//Todo Readd const edgesTable&
Route dijkstra(EdgesTable& edgesTable, const int64_t start,
               const int64_t end, const Nodes& nodes);

EdgesTable makeEdgesTable(const Graph& graph, const unsigned int size);
}
#endif // DIJKSTRA_HPP
