#ifndef BIDIRECTIONAL_DIJKSTRA_HPP
#define BIDIRECTIONAL_DIJKSTRA_HPP

#include "routerch_types.hpp"

namespace RouterCH
{

Route bidirectional_dijkstra(const EdgesTable& edgesTable, const unsigned int start,
              const unsigned int end, const Nodes& nodes);

}
#endif // BIDIRECTIONAL_DIJKSTRA_HPP
