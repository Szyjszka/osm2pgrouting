#include <algorithm>
#include <climits>
#include <iostream>
#include "contracting.hpp"
#include "ordering.hpp"
#include "CH/shortest_path_algorithms/dijkstra.hpp"

namespace RouterCH
{

bool operator ==(Route a, Route b)
{
    if(a.nodes.size() != b.nodes.size())
    {
        return false;
    }
    if(a.cost != b.cost)
    {
        return false;
    }
    for(unsigned int i = 0; i < a.nodes.size(); ++i)
    {
        if(a.nodes[i].id != b.nodes[i].id)
        {
            return false;
        }
    }
    return true;
}


uint32_t contractNode(EdgesTable& edgesTable, const Node& v, const Nodes &nodes,
                  ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable, bool addNewEdges)
{

    EdgeWithNodesTable edgeWithNodesTable(neighboursTable[v.id].size());
    Neighbours neighboursOfNode(neighboursTable[v.id]);
    for(auto neighbour : neighboursTable[v.id])
    {
//        edgeWithNodesTable[i].A = v.id;
//        edgeWithNodesTable[i].B = neighboursTable[v.id][i].id;
//        edgeWithNodesTable[i].cost = neighboursTable[v.id][i].getCost();
        neighbour.setCost(std::numeric_limits<double>::max());
        neighbour.setCost(std::numeric_limits<double>::max());
    }

    uint32_t numberOfShortcutsCreated = 0;
    // dla każdej pary (u, v) i (v,w) z krawędzi
    for(auto u : neighboursOfNode)
    {
        uint32_t uID = u.id;
        if(nodes[uID].order <= nodes[v.id].order)
        {
            continue;
        }

        for(auto w : neighboursOfNode)
        {
            uint32_t wID = w.id;
            if(nodes[wID].order <= nodes[v.id].order)
            {
                continue;
            }

            if((neighboursTable)[uID][wID].cost >=INF)
            {
                if(chechIfShortcudNeeded(neighboursTable, nodes[uID], nodes[wID], nodes,
                                        neighboursOfNode[wID].cost + neighboursOfNode[uID].cost))
                {
                    ++numberOfShortcutsCreated;

                    if(!addNewEdges)
                        continue;

                    shorctcutsTable[uID][wID].clear();
                    shorctcutsTable[wID][uID].clear();

                    neighboursTable[uID].push_back(wID);
                    neighboursTable[wID].push_back(uID);

                    EdgeWithNodes edgeWithNodes;
                    edgeWithNodes.A = uID;
                    edgeWithNodes.B = wID;
                    edgeWithNodes.cost =  neighboursOfNode[wID].cost + neighboursOfNode[uID].cost;
                    edgeWithNodesTable.push_back(edgeWithNodes);

                    //TODO shortcuts recursively from other roads
                    for(auto nodeID : shorctcutsTable[uID][v.id])
                    {
                        (shorctcutsTable)[uID][wID].push_back({nodeID});
                    }
                    (shorctcutsTable)[uID][wID].push_back({v.id});
                    for(auto nodeID : (shorctcutsTable)[v.id][wID])
                    {
                        (shorctcutsTable)[uID][wID].push_back({nodeID});
                    }
                    shorctcutsTable[wID][uID] = shorctcutsTable[uID][wID];
                    std::reverse(std::begin(shorctcutsTable[wID][uID]), std::end(shorctcutsTable[wID][uID]));
//                    std::reverse_copy(std::begin((shorctcutsTable)[uID][wID]),
//                                      std::end((shorctcutsTable)[uID][wID]),
//                                      std::begin((shorctcutsTable)[wID][uID]));
                }
            }
        }
    }

    for(auto neighbours : neighboursOfNode)
    {
         neighboursTable[v.id][neighbours.id].setCost(neighbours.getCost());
         neighboursTable[neighbours.id][v.id].setCost(neighbours.getCost());
    }
    for(auto edgeWithNodes : edgeWithNodesTable)
    {
//        edgesTable[edgeWithNodes.A][edgeWithNodes.B] = edgeWithNodes.cost;
//        edgesTable[edgeWithNodes.B][edgeWithNodes.A] = edgeWithNodes.cost;
        neighboursTable[edgeWithNodes.A].push_back(Neighbour(edgeWithNodes.B, edgeWithNodes.cost));
        neighboursTable[edgeWithNodes.B].push_back(Neighbour(edgeWithNodes.A, edgeWithNodes.cost));
    }


    return numberOfShortcutsCreated;
}

void contract(EdgesTable& edgesTable, Nodes* nodes,
              ShorctutsTable& shortcutsTable, Order &order, NeighboursTable &neighboursTable)
{
    uint32_t shortcuts = 0;
    for(uint32_t i = 0; i < order.size(); ++i)
    {
//     if(!(i%100))
        std::cout << "Zostalo jeszcze " << order.size() - i << std::endl;
//     if(!(i%5))
//         order_with_num_of_roads(nodes, &order, edgesTable, i);
//        order_with_number_of_shorctuts(nodes, &order, edgesTable, 0, shortcutsTable, neighboursTable);
     shortcuts += contractNode(edgesTable, (*nodes)[order[i]], *nodes, shortcutsTable, neighboursTable, true);
    }
}

}
