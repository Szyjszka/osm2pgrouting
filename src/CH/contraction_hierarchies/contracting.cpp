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


uint32_t contractNode(const EdgesTable& edgesTable, EdgesTable& edgesTableOut, const Node& v, const Nodes &nodes,
                  ShorctutsTable& shorctcutsTable, const uint32_t startOrder, NeighboursTable& neighboursTable)
{
    uint32_t numberOfShortcutsCreated = 0;
    // dla każdej pary (u, v) i (v,w) z krawędzi
    for(uint32_t i = 0; i < neighboursTable[v.id].size(); ++i)
    {
        uint32_t uID = neighboursTable[v.id][i];
        assert(startOrder == nodes[v.id].order);
        if(nodes[uID].order <= nodes[v.id].order)
        {
            continue;
        }

        for(uint32_t j = i + 1; j < neighboursTable[v.id].size(); ++j)
        {
            uint32_t wID = neighboursTable[v.id][j];
            if(nodes[wID].order <= nodes[v.id].order)
            {
                continue;
            }

            if((edgesTable)[uID][wID]>=INF)
            {
                //TODO Inicjalizacja na INF wyzej
                //jeśli (u,v,w) jest unikalną najkrótszą ścieżką
                if(chechIfShortcudNeeded(edgesTable, edgesTableOut, nodes[uID], v, nodes[wID], nodes))
                {
                    ++numberOfShortcutsCreated;

                    (edgesTableOut)[uID][wID] = (edgesTable)[uID][v.id] + (edgesTable)[v.id][wID];
                    (edgesTableOut)[wID][uID] = (edgesTable)[uID][v.id] + (edgesTable)[v.id][wID];
                    //Jeśli skrót już był to go usuwamy
                    if(edgesTable[uID][wID] < INF)
                    {
                        shorctcutsTable[uID][wID].clear();
                        shorctcutsTable[wID][uID].clear();
                    }
                    else
                    {
                        assert(!shorctcutsTable[uID][wID].size());
                        assert(!shorctcutsTable[wID][uID].size());

                        neighboursTable[uID].push_back(wID);
                        neighboursTable[wID].push_back(uID);
                    }

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
    return numberOfShortcutsCreated;
}

void contract(EdgesTable& edgesTable, Nodes* nodes,
              ShorctutsTable& shortcutsTable, Order &order, NeighboursTable &neighboursTable)
{
    uint32_t shortcuts = 0;
    EdgesTable edge2(edgesTable);
    for(uint32_t i = 0; i < order.size(); ++i)
    {
     std::cout << "Zostalo jeszcze " << order.size() - i << std::endl;
     //order_with_number_of_shorctuts(nodes, &order, edgesTable, 0);
     shortcuts += contractNode(edgesTable, edge2, (*nodes)[order[i]], *nodes, shortcutsTable, i, neighboursTable);

     edgesTable = edge2;
    }
}

}
