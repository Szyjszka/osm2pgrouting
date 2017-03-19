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


unsigned int contractNode(EdgesTable& edgesTable, EdgesTable& edgesTableOut, const Node& v, const Nodes &nodes,
                  ShorctutsTable& shorctcutsTable,
                  const std::map<uint32_t, int64_t>& IDconverterBack)
{
    unsigned int numberOfShortcutsCreated = 0;
    // dla każdej pary (u, v) i (v,w) z krawędzi
    for(unsigned int uID = 0; uID < (edgesTable)[0].size(); ++uID)
    {
        if(uID == v.id)
        {
            continue;
        }
        for(unsigned int wID = uID+1; wID < (edgesTable)[0].size(); ++wID)
        {
            if(wID == v.id)
            {
                continue;
            }

            //dodaj skrót (u,v,w)
                if((IDconverterBack.at(uID) == 352670061 && IDconverterBack.at(wID) == 1974837159)
                    || (IDconverterBack.at(wID) == 352670061 && IDconverterBack.at(uID) == 1974837159))
                {
//                    std::cout << "Postal taki skrot dla osm_id " << IDconverterBack.at(v.id) << std::endl;
                }
            if(((edgesTable)[uID][v.id] < UINT_MAX) && (edgesTable)[v.id][wID] < UINT_MAX && (edgesTable)[uID][wID]>=UINT_MAX)
            {
                //jeśli (u,v,w) jest unikalną najkrótszą ścieżką
                if(chechIfShortcudNeeded(edgesTable, nodes[uID], v, nodes[wID], nodes))
                {
                    ++numberOfShortcutsCreated;

                    (edgesTableOut)[uID][wID] = (edgesTable)[uID][v.id] + (edgesTable)[v.id][wID];
                    (edgesTableOut)[wID][uID] = (edgesTable)[uID][v.id] + (edgesTable)[v.id][wID];
                    //Jeśli skrót już był to go usuwamy
                    shorctcutsTable[uID][wID].clear();

                    for(auto nodeID : shorctcutsTable[uID][v.id])
                    {
                        (shorctcutsTable)[uID][wID].push_back({nodeID});
                    }
                    (shorctcutsTable)[uID][wID].push_back({v.id});
                    for(auto nodeID : (shorctcutsTable)[v.id][wID])
                    {
                        (shorctcutsTable)[uID][wID].push_back({nodeID});
                    }


                    for(auto nodeID : shorctcutsTable[uID][v.id])
                    {
                        (shorctcutsTable)[wID][uID].push_back({nodeID});
                    }
                    (shorctcutsTable)[wID][uID].push_back({v.id});
                    for(auto nodeID : (shorctcutsTable)[v.id][wID])
                    {
                        (shorctcutsTable)[wID][uID].push_back({nodeID});
                    }
                }
            }
        }
    }
    return numberOfShortcutsCreated;
}

void contract(EdgesTable& edgesTable, Nodes* nodes,
              ShorctutsTable& shortcutsTable, std::vector<unsigned int>& order,
              const std::map<uint32_t, int64_t>& IDconverterBack)
{
    unsigned int shortcuts = 0;
    EdgesTable edge2(edgesTable);
    //zakłada że nodes są w rosnącej kolejności po order
    for(signed int i = 0; i < order.size(); ++i)
    {
//        if(IDconverterBack.at(nodes[order[i]].id) == 352670061)
//        {
//            std::cout << "tu ten " <<  i << "i id " << nodes[order[i]].id <<" " << std::endl;
//        }
//     if(i && !(i % 2))
     {
        order_with_number_of_shorctuts(nodes, &order, edgesTable, i);
     }
     std::cout << "Zostalo jeszcze " << order.size() - i << std::endl;
      shortcuts += contractNode(edgesTable, edge2, (*nodes)[order[i]], *nodes, shortcutsTable, IDconverterBack);

     std::cout << "Stworzono " << shortcuts << std::endl;
     edgesTable = edge2;
    }
}

}
