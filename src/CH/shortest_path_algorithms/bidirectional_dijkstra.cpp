#include "bidirectional_dijkstra.hpp"

#include <climits>
#include <vector>
#include <cassert>
#include "dijkstra_utils.hpp"
namespace RouterCH
{

Route bidirectional_dijkstra(const EdgesTable &edgesTable, const unsigned int start,
               const unsigned int end, const Nodes& nodes)
{
    QSTable qsTableUp(nodes.size(),true);
    QSTable qsTableDown(nodes.size(),true);

    CostTable costTableUp(nodes.size(), INF);
    CostTable costTableDown(nodes.size(), INF);

    costTableUp[start] = 0;
    costTableDown[end] = 0;

    PathTable prevTable(nodes.size(),NO_PRECCESSOR);
    PathTable nextTable(nodes.size(),NO_PRECCESSOR);

    unsigned int indexOfNextElemUp  = start;
    unsigned int indexOfNextElemDown = end;
    unsigned int meetingNode = INF;
    unsigned int shortestPathLength = INF;
    unsigned int nodesLefUp = nodes.size();
    unsigned int nodesLefDown = nodes.size();

    if(start==end)
    {
        return createShortestPath(edgesTable, prevTable, start, end, nodes);
    }

    while(((costTableUp[indexOfNextElemUp] + costTableDown[indexOfNextElemDown]) <= shortestPathLength) &&
          nodesLefDown && nodesLefUp)
    {
        indexOfNextElemUp = getIndexOfNextNode(costTableUp, qsTableUp);
        --nodesLefUp;
        qsTableUp[indexOfNextElemUp] = false;
        for(unsigned int i = 0; i < nodes.size(); ++i)
        {
            if(qsTableUp[i] && edgesTable[indexOfNextElemUp][i] < INF)
            {
                if(costTableUp[i] > costTableUp[indexOfNextElemUp] + edgesTable[indexOfNextElemUp][i])
                {
                    costTableUp[i] = costTableUp[indexOfNextElemUp] + edgesTable[indexOfNextElemUp][i];
                    prevTable[i] = indexOfNextElemUp;
                    if(!qsTableDown[i])
                    {
                        if((costTableUp[i] + costTableDown[i]) < shortestPathLength)
                        {
                            meetingNode = i;
                            shortestPathLength = (costTableUp[i] + costTableDown[i]);
                        }
                    }
                }

            }
        }

        indexOfNextElemDown = getIndexOfNextNode(costTableDown, qsTableDown);
        qsTableDown[indexOfNextElemDown] = false;
        nodesLefDown--;
        for(unsigned int i = 0; i < nodes.size(); ++i)
        {
            if(qsTableDown[i] && edgesTable[indexOfNextElemDown][i] < INF)
            {
                if(costTableDown[i] > costTableDown[indexOfNextElemDown] + edgesTable[indexOfNextElemDown][i])
                {
                    costTableDown[i] = costTableDown[indexOfNextElemDown] + edgesTable[indexOfNextElemDown][i];
                    nextTable[i] = indexOfNextElemDown;
                    if(!qsTableUp[i])
                    {
                        if((costTableUp[i] + costTableDown[i]) < shortestPathLength)
                        {
                            meetingNode = i;
                            shortestPathLength = (costTableUp[i] + costTableDown[i]);
                        }
                    }
                }
            }
        }
    }

    Route route = createShortestPath(edgesTable, prevTable, start, meetingNode, nodes);
    Route routeDown = createShortestPath(edgesTable, nextTable, end, meetingNode, nodes);
    route.nodes.pop_back();
    route.nodes.insert(route.nodes.end(), routeDown.nodes.rbegin(), routeDown.nodes.rend());
    route.cost += routeDown.cost; //TODO why not costTable[s] + costTable[e] ?



    return route;
}


}
