#ifndef DIJKSTRA_UTILS_HPP
#define DIJKSTRA_UTILS_HPP

#include "../ch_types.h"

namespace RouterCH
{
typedef std::map<int64_t, bool> QSTable; //true jeśli w Q (nie został policzony)
typedef std::map<int64_t, double> CostTable;
typedef std::map<int64_t, int64_t> PathTable;

int64_t getIndexOfNextNode(CostTable& costTable, QSTable& qsTable);
Route createShortestPath(EdgesTable &edgesTable, PathTable& pathTable, const int64_t start,
                         const int64_t end, const Nodes& nodes,const ShorctutsTable* shortcutsTable = nullptr);
bool operator==(Route a, Route b);
bool operator!=(Route a, Route b);
bool operator==(Node a, Node b);
bool operator!=(Node a, Node b);
}

#endif // DIJKSTRA_UTILS_HPP
