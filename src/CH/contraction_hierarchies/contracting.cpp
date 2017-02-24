#include <algorithm>
#include <climits>
#include "contracting.hpp"
#include "../shortest_path_algorithms/dijkstra.hpp"

namespace RouterCH
{

bool chechIfShortcudNeeded(EdgesTable& edgesTable, const Node& u,
                           const Node& v, const Node& w, const Nodes &nodes)
{
    EdgesTable edgesTableForLocalSearch(edgesTable);

    //wstawienie do tabeli duzych wartosci dla poszkiwanego skrótu żeby zobaczyć czy
    //dijkstra znajdzie inną trasę
    Route checkedShortctut;
    checkedShortctut.nodes = Nodes({u, v, w});
    checkedShortctut.cost = edgesTable[u.osm_id()][v.osm_id()] + edgesTable[v.osm_id()][w.osm_id()];
    for(unsigned int i = 0; i < nodes.size(); ++i)
    {
        edgesTableForLocalSearch[nodes[i].osm_id()][v.osm_id()] = INF;
        edgesTableForLocalSearch[v.osm_id()][nodes[i].osm_id()] = INF;
    }

    Route sh = dijkstra(edgesTableForLocalSearch, u.osm_id(), w.osm_id(), nodes);

    return sh.cost > checkedShortctut.cost;
}

void contractNode(EdgesTable& edgesTable, const Node& v, const Nodes &nodes)
{
    // dla każdej pary (u, v) i (v,w) z krawędzi
    for(unsigned int uID = 0; uID < (edgesTable)[v.osm_id()].size(); ++uID)
    {
        if(uID == v.osm_id())
        {
            continue;
        }
        for(unsigned int wID = uID+1; wID < (edgesTable)[v.osm_id()].size(); ++wID)
        {
            if(wID == v.osm_id())
            {
                continue;
            }
            if(((edgesTable)[uID][v.osm_id()] < INF) && (edgesTable)[v.osm_id()][wID] < INF)
            {
                //jeśli (u,v,w) jest unikalną najkrótszą ścieżką
                if(chechIfShortcudNeeded(edgesTable, nodes[uID], v, nodes[wID], nodes))
                {
                //dodaj skrót (u,v,w)
                    (edgesTable)[uID][wID] = (edgesTable)[uID][v.osm_id()] + (edgesTable)[v.osm_id()][wID];
                    (edgesTable)[wID][uID] = (edgesTable)[uID][v.osm_id()] + (edgesTable)[v.osm_id()][wID];
                }
            }
        }
    }

}

void contract(EdgesTable& edgesTable, const Nodes& nodes)
{
    //zakłada że nodes są w rosnącej kolejności po order
    for(unsigned int i = 0; i < nodes.size(); ++i)
    {
        contractNode(edgesTable,nodes[i], nodes);
    }
}

}
