#ifndef DIJKSTRA_UTILS_HPP
#define DIJKSTRA_UTILS_HPP

#include <queue>

#include "CH/ch_types.h"

namespace RouterCH
{
typedef std::vector<bool> QSTable; //true jeśli w Q (nie został policzony)
typedef std::vector<double> CostTable;
typedef std::vector<int32_t> PathTable;

typedef std::pair<double, uint32_t> PDI;
typedef std::priority_queue<PDI, std::vector<PDI>, std::greater<PDI> > CostQue;

Route createShortestPath(const EdgesTable &edgesTable, const PathTable& pathTable, const uint32_t start,
                         const uint32_t end, const Nodes& nodes, const ShorctutsTable* shortcutsTable = nullptr);

bool chechIfShortcudNeeded(const EdgesTable& edgesTable, const Node& u,
                           const Node& w, const Nodes &nodes, const double cost, const uint32_t startingOrder);
bool operator==(const Route& a, const Route& b);
bool operator!=(const Route& a, const Route& b);
bool operator==(const Node& a, const Node& b);
bool operator!=(const Node& a, const Node& b);
}

#endif // DIJKSTRA_UTILS_HPP
