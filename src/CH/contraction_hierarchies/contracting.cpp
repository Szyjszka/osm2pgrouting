#include <algorithm>
#include <climits>
#include <iostream>
#include "contracting.hpp"
#include "../shortest_path_algorithms/dijkstra.hpp"

namespace RouterCH
{

bool chechIfShortcudNeeded(EdgesTable& edgesTable, const int64_t u,
                           const int64_t v, const int64_t w, const Nodes &nodes)
{
    EdgesTable edgesTableForLocalSearch(edgesTable);

    //wstawienie do tabeli duzych wartosci dla poszkiwanego skrótu żeby zobaczyć czy
    //dijkstra znajdzie inną trasę
    Route checkedShortctut;
    checkedShortctut.cost = edgesTable[u][v] + edgesTable[v][w];
    for(const std::pair<int64_t, Edges>& ways : edgesTable)
    for(const std::pair<int64_t, Edge>& edge : ways.second)
    {
        edgesTableForLocalSearch[ways.first][edge.first] = INF;
        edgesTableForLocalSearch[edge.first][ways.first] = INF;
    }

    Route sh = dijkstra(edgesTableForLocalSearch, u, w, nodes);

    return sh.cost > checkedShortctut.cost;
}

void contractNode(EdgesTable& edgesTable, const Node& v, const Nodes &nodes)
{
    // dla każdej pary (u, v) i (v,w) z krawędzi
    if(edgesTable.find(v.osm_id()) == edgesTable.end())
    {
        return;
    }
    for(auto i = edgesTable[v.osm_id()].begin(); i != prev(edgesTable[v.osm_id()].end()); ++i)
    for(auto j = next(i); j != edgesTable[v.osm_id()].end(); ++j)
    {
        int64_t uID = i->first;
        int64_t wID = j->first;
        if(chechIfShortcudNeeded(edgesTable, uID, v.osm_id(), wID, nodes))
        {
        //dodaj skrót (u,v,w)
            (edgesTable)[uID][wID] = (edgesTable)[uID][v.osm_id()] + (edgesTable)[v.osm_id()][wID];
            (edgesTable)[wID][uID] = (edgesTable)[uID][v.osm_id()] + (edgesTable)[v.osm_id()][wID];
        }
    }

}

void contract(EdgesTable& edgesTable, const Nodes& nodes)
{
    //zakłada że nodes są w rosnącej kolejności po order
    for(unsigned int i = 0; i < nodes.size(); ++i)
    {
        std::cout << "Jeszcze " << nodes.size() - i << std::endl;
        contractNode(edgesTable,nodes[i], nodes);
    }
}

}
