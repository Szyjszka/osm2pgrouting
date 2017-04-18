#include "modified_bidirectional_dijkstra.hpp"

#include <climits>
#include <vector>
#include <cassert>
#include "dijkstra_utils.hpp"
namespace RouterCH
{

Route modified_bidirectional_dijkstra(const EdgesTable &edgesTable, const uint32_t start,
                                      const uint32_t end, const Nodes& nodes, const ShorctutsTable& shortcutsTable)
{
    QSTable qsTableUp(nodes.size(),true);
    QSTable qsTableDown(nodes.size(),true);

    CostTable costTableUp(nodes.size(), INF);
    CostTable costTableDown(nodes.size(), INF);

    costTableUp[start] = 0;
    costTableDown[end] = 0;

    PathTable prevTable(nodes.size(),NO_PRECCESSOR);
    PathTable nextTable(nodes.size(),NO_PRECCESSOR);

    uint32_t indexOfNextElemUp  = start;
    uint32_t indexOfNextElemDown = end;
    uint32_t meetingNode = INF;
    double shortestPathLength = INF;
    size_t nodesLefUp = nodes.size();
    size_t nodesLefDown = nodes.size();

    if(start==end)
    {
        return createShortestPath(edgesTable, prevTable, start, end, nodes);
    }

    while(((costTableUp[indexOfNextElemUp] + costTableDown[indexOfNextElemDown]) <= shortestPathLength) &&
          nodesLefDown && nodesLefUp)
    {
        //TO be fixed with priority que
        assert(false);
        uint32_t indexOfNextElemUpExp = 0;//getIndexOfNextNode(costTableUp, qsTableUp);
        uint32_t indexOfNextElemDownExp = 0;//getIndexOfNextNode(costTableDown, qsTableDown);
        bool Up = costTableDown[indexOfNextElemDownExp] > costTableUp[indexOfNextElemUpExp];
        if(Up){
        --nodesLefUp;
        indexOfNextElemUp = indexOfNextElemUpExp;
        if(costTableUp[indexOfNextElemUp] < INF)
        {
            qsTableUp[indexOfNextElemUp] = false;
            for(uint32_t i = 0; i < nodes.size(); ++i)
            {
                if(qsTableUp[i] && (edgesTable[indexOfNextElemUp].at(i) < INF) &&
                        (nodes[i].order > nodes[indexOfNextElemUp].order))
                {
                        if(costTableUp[i] > costTableUp[indexOfNextElemUp] + edgesTable[indexOfNextElemUp].at(i) )
                        {
                            costTableUp[i] = costTableUp[indexOfNextElemUp] + edgesTable[indexOfNextElemUp].at(i) ;
                            prevTable[i] = indexOfNextElemUp;
                            if(costTableDown[i] < INF)
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
        }
        else{
        nodesLefDown--;
        indexOfNextElemDown = indexOfNextElemDownExp;
        if(costTableDown[indexOfNextElemDown] < INF)
        {
            qsTableDown[indexOfNextElemDown] = false;
            for(uint32_t i = 0; i < nodes.size(); ++i)
            {
                if(qsTableDown[i] && (edgesTable[indexOfNextElemDown].at(i)  < INF) &&
                        (nodes[i].order > nodes[indexOfNextElemDown].order))
                {
                    if(costTableDown[i] > costTableDown[indexOfNextElemDown] + edgesTable[indexOfNextElemDown].at(i))
                    {
                        costTableDown[i] = costTableDown[indexOfNextElemDown] + edgesTable[indexOfNextElemDown].at(i);
                        nextTable[i] = indexOfNextElemDown;
                        if(costTableUp[i] < INF)
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
        }
    }
    if(meetingNode == INF)
    {
        Route route;
        route.cost = INF;
        route.id = 0;
        return route;
    }

    Route route = createShortestPath(edgesTable, prevTable, start, meetingNode, nodes, &shortcutsTable);
    Route routeDown = createShortestPath(edgesTable, nextTable, end, meetingNode, nodes, &shortcutsTable);
    route.nodes.pop_back();
    route.nodes.insert(route.nodes.end(), routeDown.nodes.rbegin(), routeDown.nodes.rend());
    route.cost += routeDown.cost; //TODO why not costTable[s] + costTable[e] ?



    return route;
}


}
