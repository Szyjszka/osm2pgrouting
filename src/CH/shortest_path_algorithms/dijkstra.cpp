#include "dijkstra.hpp"

#include <climits>
#include <vector>
#include <cassert>
#include "dijkstra_utils.hpp"

namespace RouterCH
{

Route dijkstra(const NeighboursTable& neighboursTable, const uint32_t start,
               const uint32_t end, const Nodes& nodes, const double maxCost)
{

    QSTable qsTable(nodes.size(),true);
    CostTable costTable(nodes.size(), std::numeric_limits<double>::max());
    costTable[start] = 0;
    PathTable pathTable(nodes.size(), NO_PRECCESSOR);
    Route returnedRoute;
    returnedRoute.cost = std::numeric_limits<double>::max();
    returnedRoute.id = 0;
    size_t nodesLeft = nodes.size();
    uint32_t indexOfNextElem = start;

    while(nodesLeft && indexOfNextElem != end )
    {
        indexOfNextElem = getIndexOfNextNode(costTable, qsTable);
        if(costTable[indexOfNextElem] > maxCost)
        {
            return returnedRoute;
        }
        qsTable[indexOfNextElem] = false;
        for(auto neighbour : neighboursTable[indexOfNextElem])
        {
            if(qsTable[neighbour.id] && neighbour.getCost() < INF)
            {
                if(costTable[neighbour.id] > costTable[indexOfNextElem] + neighbour.getCost())
                {
                    costTable[neighbour.id] = costTable[indexOfNextElem] + neighbour.getCost();
                    pathTable[neighbour.id] = indexOfNextElem;
                }

            }
        }
        --nodesLeft;
    }

    if(indexOfNextElem != end)
    {
        return returnedRoute;
    }

    returnedRoute.cost = costTable[end];
    return returnedRoute;
//    return createShortestPath(edgesTable, pathTable, start,end,nodes);
}

}
