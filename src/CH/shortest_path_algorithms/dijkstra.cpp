#include "dijkstra.hpp"

#include <climits>
#include <vector>
#include <cassert>
#include "dijkstra_utils.hpp"

namespace RouterCH
{

Route dijkstra(const EdgesTable &edgesTable, const unsigned int start,
               const unsigned int end, const Nodes& nodes)
{

    QSTable qsTable(nodes.size(),true);
    CostTable costTable(nodes.size(), INF);
    costTable[start] = 0;
    PathTable pathTable(nodes.size(), NO_PRECCESSOR);
    Route invalidRoute;
    invalidRoute.cost = INF;
    invalidRoute.id = 0;
    unsigned int nodesLeft = nodes.size();
    unsigned int indexOfNextElem = start;

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
