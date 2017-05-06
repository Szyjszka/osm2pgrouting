#include <climits>
#include <cassert>
#include "dijkstra_utils.hpp"
#include "dijkstra.hpp"

namespace RouterCH
{

bool operator==(const Node& a, const Node& b)
{
    return a.id == b.id && a.order == b.order && a.orderPoints == b.orderPoints;
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

Route createShortestPath(const EdgesTable &edgesTable, const PathTable& pathTable, const uint32_t start,
                         const uint32_t end, const Nodes& nodes, const ShorctutsTable* shortcutsTable)
{
    Route shortestPath;
    shortestPath.id = 0; //TODO consider other solution
    shortestPath.cost = 0;
    uint32_t indexOfNext = end;
    uint32_t leftIter = INF;
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
            size_t shortcutSize = (*shortcutsTable)[indexOfNext].at(pathTable[indexOfNext]).size();
            if(shortcutSize)
            {
                for(uint32_t j = 0; j < shortcutSize; ++j)
                {
                    shortestPath.nodes.push_back(nodes[(*shortcutsTable)[indexOfNext].at(pathTable[indexOfNext])[shortcutSize-j-1]]);
                }
            }
        }
        shortestPath.cost += edgesTable[indexOfNext].at(pathTable[indexOfNext]);

        indexOfNext = pathTable[indexOfNext];

        --leftIter;
        assert(leftIter);
    }
    shortestPath.nodes.push_back(nodes[indexOfNext]);

    std::reverse(shortestPath.nodes.begin(), shortestPath.nodes.end());
    return shortestPath;
}



bool chechIfShortcudNeeded(const EdgesTable& edgesTable, const Node& u,
                           const Node& w, const Nodes &nodes, const double cost, const uint32_t startingOrder)
{
    Route sh = dijkstra(edgesTable, u.id, w.id, nodes, cost, startingOrder);

    return sh.cost > cost && (cost < edgesTable[u.id].at(w.id) || edgesTable[u.id].at(w.id) >= INF);
}
}
