#ifndef DIJKSTRA_UTILS_HPP
#define DIJKSTRA_UTILS_HPP

#include "../ch_types.h"

namespace RouterCH
{
typedef std::vector<bool> QSTable; //true jeśli w Q (nie został policzony)
typedef std::vector<double> CostTable;
typedef std::vector<int64_t> PathTable;

unsigned int getIndexOfNextNode(CostTable& costTable, const QSTable& qsTable);
Route createShortestPath(EdgesTable &edgesTable, const PathTable& pathTable, const int64_t start,
                         const int64_t end, const Nodes& nodes,const ShorctutsTable* shortcutsTable = nullptr);
bool operator==(Route a, Route b);
bool operator!=(Route a, Route b);
bool operator==(Node a, Node b);
bool operator!=(Node a, Node b);
}

#endif // DIJKSTRA_UTILS_HPP
