#include "dijkstra.hpp"

#include <climits>
#include <vector>
#include <cassert>
#include "dijkstra_utils.hpp"

namespace RouterCH
{

Route dijkstra(const EdgesTable &edgesTable, const uint32_t start,
               const uint32_t end, const Nodes& nodes, const double maxCost, const uint32_t startingOrder)
{
    QSTable qsTable(nodes.size(),true);
    CostQue costQue;
    costQue.push(std::make_pair(0.0, start));
    CostTable costTable(nodes.size(), std::numeric_limits<double>::max());
    costTable[start] = 0;
    Route returnedRoute;
    returnedRoute.cost = std::numeric_limits<double>::max();
    returnedRoute.id = 0;
    PDI nextElem = costQue.top();

    while(costQue.size() && nextElem.second != end)
    {
        do{
            nextElem = costQue.top();
            costQue.pop();
        }while(!qsTable[nextElem.second] && costQue.size());

        if(nextElem.first > maxCost)
        {
            return returnedRoute;
        }
        qsTable[nextElem.second] = false;
        for(auto edge : edgesTable[nextElem.second])
        {
                if(qsTable[edge.first] && edge.second  < INF && nodes[edge.first].order >= startingOrder)
                {
                    if(costTable[edge.first] > costTable[nextElem.second] + edge.second)
                    {
                        costTable[edge.first] = costTable[nextElem.second] + edge.second;
                        if(qsTable[edge.first])
                        {
                            costQue.push(std::make_pair(costTable[edge.first], edge.first));
                        }
                    }

                }
        }
    }

    if(nextElem.second != end)
    {
        return returnedRoute;
    }

    returnedRoute.cost = costTable[end];
    return returnedRoute;
}

}
