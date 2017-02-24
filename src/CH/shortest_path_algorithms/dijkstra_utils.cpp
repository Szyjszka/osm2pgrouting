#include <climits>
#include <cassert>
#include "dijkstra_utils.hpp"

namespace RouterCH
{
bool operator==(Node a, Node b)
{
    if(a.point_geometry() != b.point_geometry())
    {
        return false;
    }
    if(a.osm_id() != b.osm_id())
    {
        return false;
    }
    return true;
}

bool operator !=(Node a, Node b)
{
    return !(a==b);
}

bool operator==(Route a, Route b)
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

bool operator!=(Route a, Route b)
{
    return !(a==b);
}

unsigned int getIndexOfNextNode(CostTable& costTable, const QSTable& qsTable)
{
    double minDistance = std::numeric_limits<double>::max();
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
//Todo readd const edgedTable&
Route createShortestPath(EdgesTable &edgesTable, const PathTable& pathTable, const int64_t start,
                         const int64_t end, const Nodes& nodes, const ShorctutsTable* shortcutsTable)
{
    Route shortestPath;
    shortestPath.id = 0; //TODO consider other solution
    shortestPath.cost = 0;
    int64_t indexOfNext = end;
    int64_t leftIter = UINT_MAX;
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
