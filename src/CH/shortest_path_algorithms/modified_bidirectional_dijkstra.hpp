#ifndef MODIFIED_BIDIRECTIONAL_DIJKSTRA_HPP
#define MODIFIED_BIDIRECTIONAL_DIJKSTRA_HPP

#include "../ch_types.h"

namespace RouterCH
{

Route modified_bidirectional_dijkstra(const EdgesTable& edgesTable, const unsigned int start,
              const unsigned int end, const Nodes& nodes, const ShorctutsTable& shortcutsTable);

}
#endif // MODIFIED_BIDIRECTIONAL_DIJKSTRA_HPP
