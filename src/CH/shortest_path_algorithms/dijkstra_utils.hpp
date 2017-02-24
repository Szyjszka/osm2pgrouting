#ifndef DIJKSTRA_UTILS_HPP
#define DIJKSTRA_UTILS_HPP

#include "../ch_types.h"

namespace RouterCH
{
typedef std::vector<bool> QSTable; //true jeśli w Q (nie został policzony)
typedef std::vector<unsigned int> CostTable;
typedef std::vector<signed int> PathTable;

unsigned int getIndexOfNextNode(CostTable& costTable, const QSTable& qsTable);
Route createShortestPath(EdgesTable &edgesTable, const PathTable& pathTable, const unsigned int start,
                         const unsigned int end, const Nodes& nodes,const ShorctutsTable* shortcutsTable = nullptr);
bool operator==(Route a, Route b);
bool operator!=(Route a, Route b);
bool operator==(Node a, Node b);
bool operator!=(Node a, Node b);
}

#endif // DIJKSTRA_UTILS_HPP
