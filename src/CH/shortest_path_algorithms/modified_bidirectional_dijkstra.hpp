#ifndef MODIFIED_BIDIRECTIONAL_DIJKSTRA_HPP
#define MODIFIED_BIDIRECTIONAL_DIJKSTRA_HPP

#include "../ch_types.h"

namespace RouterCH
{

Route modified_bidirectional_dijkstra(const EdgesTable& edgesTable, const uint32_t start,
              const uint32_t end, const Nodes& nodes, const ShorctutsTable& shortcutsTable);

}
#endif // MODIFIED_BIDIRECTIONAL_DIJKSTRA_HPP
