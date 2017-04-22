#include <algorithm>
#include <climits>
#include <iostream>
#include "contracting.hpp"
#include "ordersupervisor.hpp"
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
                  ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable, bool addNewEdges,  ShorctutsInfoTable& shortcutInfos)
{

    EdgeWithNodesTable edgeWithNodesTable(neighboursTable[v.id].size());
    static uint32_t shortcutID = edgesTable.size()*edgesTable.size();
    for(uint32_t i = 0; i < neighboursTable[v.id].size(); ++i)
    {
        edgeWithNodesTable[i].A = v.id;
        edgeWithNodesTable[i].B = neighboursTable[v.id][i];
        edgeWithNodesTable[i].cost = edgesTable[neighboursTable[v.id][i]].at(v.id);
        edgesTable[neighboursTable[v.id][i]][v.id] = std::numeric_limits<double>::max();
        edgesTable[v.id][neighboursTable[v.id][i]] = std::numeric_limits<double>::max();
    }

    uint32_t numberOfShortcutsCreated = 0;
    // dla każdej pary (u, v) i (v,w) z krawędzi
    for(uint32_t i = 0; i < neighboursTable[v.id].size(); ++i)
    {
        uint32_t uID = neighboursTable[v.id][i];
        //TODO shouldn't this order set be -1 ?
        //order 0 for shortcut ordering
        assert(nodes[uID].order != nodes[v.id].order ||  nodes[v.id].order==0);
        if(nodes[uID].order <= nodes[v.id].order)
        {
            continue;
        }

        for(uint32_t j = i + 1; j < neighboursTable[v.id].size(); ++j)
        {
            uint32_t wID = neighboursTable[v.id][j];
            assert(nodes[wID].order != nodes[v.id].order ||  nodes[v.id].order==0);
            if(nodes[wID].order <= nodes[v.id].order)
            {
                continue;
            }

            if((edgesTable)[uID].at(wID)>=INF)
            {
                if(chechIfShortcudNeeded(edgesTable, nodes[uID], nodes[wID], nodes,
                                        edgeWithNodesTable[i].cost + edgeWithNodesTable[j].cost))
                {
                    ++numberOfShortcutsCreated;

                    if(!addNewEdges)
                        continue;

                    shorctcutsTable[uID][wID].clear();
                    shorctcutsTable[wID][uID].clear();

                    EdgeWithNodes edgeWithNodes;
                    edgeWithNodes.A = uID;
                    edgeWithNodes.B = wID;
                    edgeWithNodes.cost =  edgeWithNodesTable[i].cost + edgeWithNodesTable[j].cost;
                    edgeWithNodesTable.push_back(edgeWithNodes);

                    if(shortcutInfos[uID].find(v.id) != shortcutInfos[uID].end())
                    {
                        shortcutInfos[uID][wID].shA =shortcutInfos[uID][v.id].id;
                        shortcutInfos[wID][uID].shB =shortcutInfos[uID][v.id].id;
                    }
                    else
                    {
                        shortcutInfos[uID][wID].shA = uID*nodes.size() + wID;
                        shortcutInfos[wID][uID].shB = uID*nodes.size() + wID;
                    }

                    if(shortcutInfos[wID].find(v.id) != shortcutInfos[wID].end())
                    {
                        shortcutInfos[uID][wID].shB =shortcutInfos[wID][v.id].id;
                        shortcutInfos[wID][uID].shA =shortcutInfos[wID][v.id].id;
                    }
                    else
                    {
                        shortcutInfos[uID][wID].shB = wID*nodes.size() + uID;
                        shortcutInfos[wID][uID].shA = wID*nodes.size() + uID;
                    }

                    shortcutInfos[uID][wID].id = shortcutID;
                    shortcutInfos[wID][uID].id = shortcutID;
                    ++shortcutID;

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

                    neighboursTable[uID].push_back(wID);
                    neighboursTable[wID].push_back(uID);
                }
            }
        }
    }
    for(auto edgeWithNodes : edgeWithNodesTable)
    {
        edgesTable[edgeWithNodes.A][edgeWithNodes.B] = edgeWithNodes.cost;
        edgesTable[edgeWithNodes.B][edgeWithNodes.A] = edgeWithNodes.cost;
    }


    return numberOfShortcutsCreated;
}

void contract(EdgesTable& edgesTable, Nodes& nodes,
              ShorctutsTable& shortcutsTable, NeighboursTable &neighboursTable, ShorctutsInfoTable &shortcutInfos)
{
    uint32_t shortcuts = 0;
    OrderSupervisor orderSupervisor(OrderSupervisor::Strategy::LazyUpdate, OrderCriterium::Shortcuts,
                                    nodes, edgesTable, neighboursTable, shortcutsTable);
    for(uint32_t i = 0; i < nodes.size(); ++i)
    {
        orderSupervisor.updateOrder(nodes, edgesTable, neighboursTable, shortcutsTable);
        uint32_t nextNode = orderSupervisor.getIndexOfNextNode();
        if(!(i%100))
        {
            std::cout << "Zostalo jeszcze " << nodes.size() - i << " Utworzono "<< shortcuts << " skrotow" << std::endl;
        }
        shortcuts += contractNode(edgesTable, nodes[nextNode], nodes, shortcutsTable, neighboursTable, true, shortcutInfos);


    }
}

}
