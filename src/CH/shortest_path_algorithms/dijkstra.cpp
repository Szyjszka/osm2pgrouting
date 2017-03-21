#include "dijkstra.hpp"

#include <climits>
#include <vector>
#include <cassert>
#include "dijkstra_utils.hpp"

namespace RouterCH
{

Route dijkstra(const EdgesTable &edgesTable, const uint32_t start,
               const uint32_t end, const Nodes& nodes)
{

    QSTable qsTable(nodes.size(),true);
    CostTable costTable(nodes.size(), std::numeric_limits<double>::max());
    costTable[start] = 0;
    PathTable pathTable(nodes.size(), NO_PRECCESSOR);
    Route invalidRoute;
    invalidRoute.cost = std::numeric_limits<double>::max();
    invalidRoute.id = 0;
    size_t nodesLeft = nodes.size();
    uint32_t indexOfNextElem = start;

    while(nodesLeft && indexOfNextElem != end )
    {
        indexOfNextElem = getIndexOfNextNode(costTable, qsTable);
        if(costTable[indexOfNextElem] > INF)
        {
            return invalidRoute;
        }
        qsTable[indexOfNextElem] = false;
        for(uint32_t i = 0; i < nodes.size(); ++i)
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

    return createShortestPath(edgesTable, pathTable, start,end,nodes);
}

}
