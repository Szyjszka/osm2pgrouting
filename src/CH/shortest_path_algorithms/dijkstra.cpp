#include "dijkstra.hpp"

#include <climits>
#include <vector>
#include <cassert>
#include "dijkstra_utils.hpp"

namespace RouterCH
{

bool distanceIsSmallerFromB(const Node& target, const Node& A, const Node& B)
{
      bool lat  = std::abs(B.lat - target.lat) < std::abs(A.lat - target.lat);
      bool lon  = std::abs(B.lon - target.lon) < std::abs(A.lon - target.lon);
      return lat || lon;
//    double diffA = pow(A.lat - target.lat, 2) + pow(A.lon - target.lon, 2);
//    double diffB = pow(B.lat - target.lat, 2) + pow(B.lon - target.lon, 2);
//    return diffA < diffB;
}

Route dijkstra(const EdgesTable &edgesTable, const uint32_t start,
               const uint32_t end, const Nodes& nodes, const double maxCost, const uint32_t startingOrder,
               const uint32_t maxHop, const uint32_t maxSettledNodes)
{
    QSTable qsTable(nodes.size(),true);
//    PathTable numberOfWaysAway(nodes.size(), 0);
//    uint32_t settledNodes = 0;
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

//        ++settledNodes;
        if(nextElem.first > maxCost/* || settledNodes > maxSettledNodes || numberOfWaysAway[nextElem.second] > maxHop*/)
        {
            return returnedRoute;
        }
        qsTable[nextElem.second] = false;
        for(auto edge : edgesTable[nextElem.second])
        {
                if(qsTable[edge.first] && edge.second  < INF && nodes[edge.first].order >= startingOrder)
                {
                    if(distanceIsSmallerFromB(nodes[end], nodes[nextElem.second], nodes[edge.first]))
                    if(costTable[edge.first] > costTable[nextElem.second] + edge.second)
                    {
//                        numberOfWaysAway[edge.first] = numberOfWaysAway[nextElem.second] + 1;
                        costTable[edge.first] = costTable[nextElem.second] + edge.second;
//                        numberOfWaysAway[edge.first] = numberOfWaysAway[nextElem.second] + 1;
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
