#include "dijkstra.hpp"

#include <climits>
#include <vector>
#include <cassert>
#include "dijkstra_utils.hpp"

namespace RouterCH
{

//TODO readd const& to edgesTable
Route dijkstra(EdgesTable &edgesTable, const int64_t start,
               const int64_t end, const Nodes& nodes)
{
    QSTable qsTable;
    CostTable costTable;
    PathTable pathTable;
    for(const Node& n : nodes)
    {
        qsTable[n.osm_id()] = true;
        costTable[n.osm_id()] = INF;
        pathTable[n.osm_id()] = NO_PRECCESSOR;
    }
    costTable[start] = 0;
    Route invalidRoute;
    invalidRoute.cost = INF;
    invalidRoute.id = 0;
    int64_t nodesLeft = nodes.size();
    int64_t indexOfNextElem = start;

    while(nodesLeft && indexOfNextElem != end )
    {
        indexOfNextElem = getIndexOfNextNode(costTable, qsTable);
        if(costTable[indexOfNextElem] == INF)
        {
            return invalidRoute;
        }
        qsTable[indexOfNextElem] = false;
        for(unsigned int i = 0; i < nodes.size(); ++i)
        {
            if(qsTable[i] && edgesTable[indexOfNextElem][i] < INF)
            {
                if(costTable[i] > costTable[indexOfNextElem] + edgesTable[indexOfNextElem][i])
                {
                    costTable[i] = costTable[indexOfNextElem] + edgesTable[indexOfNextElem][i];
                    pathTable[i] = indexOfNextElem;
                }

            }
        }
        --nodesLeft;
    }

    if(indexOfNextElem != end)
    {
        return invalidRoute;
    }

    return createShortestPath(edgesTable, pathTable,start,end,nodes);
}

}
