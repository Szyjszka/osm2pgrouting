#include <algorithm>
#include <climits>
#include <iostream>
#include "contracting.hpp"
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

bool chechIfShortcudNeeded(const EdgesTable& edgesTable, const Node& u,
                           const Node& v, const Node& w, const Nodes &nodes)
{
    //To może być wyżej
    EdgesTable edgesTableForLocalSearch(edgesTable);

    //wstawienie do tabeli duzych wartosci dla poszkiwanego skrótu żeby zobaczyć czy
    //dijkstra znajdzie inną trasę
    Route checkedShortctut;
    checkedShortctut.nodes = Nodes({u, v, w});
    checkedShortctut.cost = edgesTable[u.id][v.id] + edgesTable[v.id][w.id];
    for(unsigned int i = 0; i < nodes.size(); ++i)
    {
        edgesTableForLocalSearch[nodes[i].id][v.id] = INF;
        edgesTableForLocalSearch[v.id][nodes[i].id] = INF;
    }

    Route sh = dijkstra(edgesTableForLocalSearch, u.id, w.id, nodes);

    return sh.cost > checkedShortctut.cost;
}

void contractNode(EdgesTable& edgesTable, const Node& v, const Nodes &nodes,
                  ShorctutsTable& shorctcutsTable,
                  const std::map<uint32_t, int64_t>& IDconverterBack)
{
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
            if(((edgesTable)[uID][v.id] < UINT_MAX) && (edgesTable)[v.id][wID] < UINT_MAX)
            {
                //if((IDconverterBack.at(uID) == 3291116323 && IDconverterBack.at(wID) == 352670061)
                //    || (IDconverterBack.at(wID) == 3291116323 && IDconverterBack.at(uID) == 352670061))
                //{
                //    std::cout << "Som takie krawedzie" << std::endl;
                //}
                //jeśli (u,v,w) jest unikalną najkrótszą ścieżką
                if(chechIfShortcudNeeded(edgesTable, nodes[uID], v, nodes[wID], nodes))
                {
                //dodaj skrót (u,v,w)
                    (edgesTable)[uID][wID] = (edgesTable)[uID][v.id] + (edgesTable)[v.id][wID];
                    (edgesTable)[wID][uID] = (edgesTable)[uID][v.id] + (edgesTable)[v.id][wID];
                    //Jeśli skrót już był to go usuwamy
                    (shorctcutsTable)[uID][wID].clear();
                    (shorctcutsTable)[wID][uID].clear();

                    (shorctcutsTable)[uID][wID].push_back({uID});
                    for(auto nodeID : shorctcutsTable[uID][v.id])
                    {
                        (shorctcutsTable)[uID][wID].push_back({nodeID});
                    }
                    (shorctcutsTable)[uID][wID].push_back({v.id});
                    for(auto nodeID : (shorctcutsTable)[v.id][wID])
                    {
                        (shorctcutsTable)[uID][wID].push_back({nodeID});
                    }
                    (shorctcutsTable)[uID][wID].push_back({wID});


                    (shorctcutsTable)[wID][uID].push_back({wID});
                    for(auto nodeID : shorctcutsTable[uID][v.id])
                    {
                        (shorctcutsTable)[wID][uID].push_back({nodeID});
                    }
                    (shorctcutsTable)[wID][uID].push_back({v.id});
                    for(auto nodeID : (shorctcutsTable)[v.id][wID])
                    {
                        (shorctcutsTable)[wID][uID].push_back({nodeID});
                    }
                    (shorctcutsTable)[wID][uID].push_back({uID});
                }
            }
        }
    }

}

void contract(EdgesTable& edgesTable, const Nodes& nodes,
              ShorctutsTable& shortcutsTable, const std::vector<unsigned int>& order,
              const std::map<uint32_t, int64_t>& IDconverterBack)
{
    //zakłada że nodes są w rosnącej kolejności po order
    for(unsigned int i = 0; i < order.size(); ++i)
    {
        if(IDconverterBack.at(nodes[order[i]].id) == 352670061)
        {
            std::cout << " to ten" << std::endl;
        }
        std::cout << "Zostalo jeszcze " << nodes.size() - i << std::endl;
        contractNode(edgesTable, nodes[order[i]], nodes, shortcutsTable, IDconverterBack);
    }
}

}
