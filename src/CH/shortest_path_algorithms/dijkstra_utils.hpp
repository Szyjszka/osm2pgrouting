#ifndef DIJKSTRA_UTILS_HPP
#define DIJKSTRA_UTILS_HPP

#include "routerch_types.hpp"

namespace RouterCH
{
typedef std::vector<bool> QSTable; //true jeśli w Q (nie został policzony)
typedef std::vector<unsigned int> CostTable;
typedef std::vector<signed int> PathTable;

EdgesTable makeEdgesTable(const Graph& graph, const unsigned int size);
unsigned int getIndexOfNextNode(CostTable& costTable, const QSTable& qsTable);
Route createShortestPath(const EdgesTable &edgesTable, const PathTable& pathTable, const unsigned int start,
                         const unsigned int end, const Nodes& nodes,const ShorctutsTable* shortcutsTable = nullptr);
bool operator==(const Route& a, const Route& b);
bool operator!=(const Route& a, const Route& b);
bool operator==(const Node& a, const Node& b);
bool operator!=(const Node& a, const Node& b);
}

#endif // DIJKSTRA_UTILS_HPP
