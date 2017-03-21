#include <climits>
#include <cassert>
#include "dijkstra_utils.hpp"
#include "dijkstra.hpp"

namespace RouterCH
{

bool operator==(const Node& a, const Node& b)
{
    return a.id == b.id;
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

uint32_t getIndexOfNextNode(CostTable& costTable, const QSTable& qsTable)
{
    double minDistance = std::numeric_limits<double>::max();
    uint32_t indexOfMinDistance = 0;

    for(uint32_t i = 0; i < costTable.size(); ++i)
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
            size_t shortcutSize = (*shortcutsTable)[indexOfNext][pathTable[indexOfNext]].size();
            if(shortcutSize)
            {
                for(uint32_t j = 0; j < shortcutSize; ++j)
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



bool chechIfShortcudNeeded(const EdgesTable& edgesTable, const Node& u,
                           const Node& v, const Node& w, const Nodes &nodes)
{
    //To może być wyżej
    EdgesTable edgesTableForLocalSearch(edgesTable);

    //wstawienie do tabeli duzych wartosci dla poszkiwanego skrótu żeby zobaczyć czy
    //dijkstra znajdzie inną trasę
    Route checkedShortctut;
    checkedShortctut.nodes = Nodes({u, v, w});
    checkedShortctut.cost = edgesTable[u.id][v.id] + edgesTable[v.id][w.id];
    for(uint32_t i = 0; i < nodes.size(); ++i)
    {
        edgesTableForLocalSearch[nodes[i].id][v.id] = std::numeric_limits<double>::max();
        edgesTableForLocalSearch[v.id][nodes[i].id] = std::numeric_limits<double>::max();
    }

    Route sh = dijkstra(edgesTableForLocalSearch, u.id, w.id, nodes);

    return sh.cost > checkedShortctut.cost;
}
}
