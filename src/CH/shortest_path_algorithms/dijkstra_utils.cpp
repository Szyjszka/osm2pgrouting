#include <climits>
#include <cassert>
#include "dijkstra_utils.hpp"

namespace RouterCH
{

EdgesTable makeEdgesTable(const Graph& graph, const unsigned int size)
{
    EdgesTable edgesTable(size, std::vector<unsigned int>(size, INF));

    for(unsigned int i = 0; i < edgesTable.size(); ++i)
    {
        edgesTable[i][i] = 0;
    }

    for(const auto& route : graph)
    {
        edgesTable[route.nodes[0].id][route.nodes.back().id] = route.cost;
        edgesTable[route.nodes.back().id][route.nodes[0].id] = route.cost;
    }

    return edgesTable;
}

bool operator==(const Node& a, const Node& b)
{
    if(a.coord != b.coord)
    {
        return false;
    }
    if(a.id != b.id)
    {
        return false;
    }
    return true;
}

bool operator !=(const Node& a, const Node& b)
{
    return !(a==b);
}

bool operator==(const Route& a, const Route& b)
{
    if(a.cost != b.cost)
    {
        return false;
    }
    if(a.nodes.front() != b.nodes.front())
    {
        return false;
    }
    if(a.nodes.back() != b.nodes.back())
    {
        return false;
    }
    return true;
}

bool operator!=(const Route& a, const Route& b)
{
    return !(a==b);
}

unsigned int getIndexOfNextNode(CostTable& costTable, const QSTable& qsTable)
{
    uint64_t minDistance = UINT64_MAX;
    unsigned int indexOfMinDistance = 0;

    for(unsigned int i = 0; i < costTable.size(); ++i)
    {
        if(qsTable[i])
        {
            if(minDistance > costTable[i])
            {
                minDistance = costTable[i];
                indexOfMinDistance = i;
            }
        }
    }
    return indexOfMinDistance;
}

Route createShortestPath(const EdgesTable &edgesTable, const PathTable& pathTable, const unsigned int start,
                         const unsigned int end, const Nodes& nodes, const ShorctutsTable* shortcutsTable)
{
    Route shortestPath;
    shortestPath.id = 0; //TODO consider other solution
    shortestPath.cost = 0;
    unsigned int indexOfNext = end;
    unsigned int leftIter = UINT_MAX;
    if(start == end)
    {
        shortestPath.nodes.push_back(nodes[end]);
        return shortestPath;
    }
    if(indexOfNext == INF)
    {
        return shortestPath;
    }
    while(pathTable[indexOfNext] != -1)
    {

        shortestPath.nodes.push_back(nodes[indexOfNext]);
        if(shortcutsTable)
        {
            unsigned int shortcutSize = (*shortcutsTable)[indexOfNext][pathTable[indexOfNext]].size();
            if(shortcutSize)
            {
                for(unsigned int j = 0; j < shortcutSize; ++j)
                {
                    shortestPath.nodes.push_back(nodes[(*shortcutsTable)[indexOfNext][pathTable[indexOfNext]][shortcutSize-j-1]]);
                }
            }
        }
        shortestPath.cost += edgesTable[indexOfNext][pathTable[indexOfNext]];

        indexOfNext = pathTable[indexOfNext];

        --leftIter;
        assert(leftIter);
    }
    shortestPath.nodes.push_back(nodes[indexOfNext]);

    std::reverse(shortestPath.nodes.begin(), shortestPath.nodes.end());
    return shortestPath;
}
}
