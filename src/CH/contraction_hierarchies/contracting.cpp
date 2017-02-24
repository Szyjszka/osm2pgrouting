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
    Route checkedShortctut;
    checkedShortctut.cost = edgesTable[u][v] + edgesTable[v][w];

    Route sh = dijkstra(edgesTable, u, w, nodes, checkedShortctut.cost);

    return sh.cost > checkedShortctut.cost;
}

void contractNode(EdgesTable& edgesTable, const Node& v, const Nodes &nodes, std::map<int64_t, osm2pgr::Way>& newWays)
{
    // dla każdej pary (u, v) i (v,w) z krawędzi
    if(edgesTable.find(v.osm_id()) == edgesTable.end())
    {
        return;
    }

    Edges savedEdges;
    std::vector<int64_t> connectedNodes;
    //erase connections
    for(std::pair<int64_t, Edge> elem : edgesTable[v.osm_id()])
    {
        savedEdges[elem.first] = elem.second;
        edgesTable[elem.first].erase(v.osm_id());
        edgesTable[v.osm_id()].erase(elem.first);
        connectedNodes.push_back(elem.first);
    }

    for(auto i = connectedNodes.begin(); i != prev(connectedNodes.end()); ++i)
    for(auto j = next(i); j != connectedNodes.end(); ++j)
    {

        if(chechIfShortcudNeeded(edgesTable, *i , v.osm_id(), *j, nodes))
        {
        //dodaj skrót (u,v,w)
            (edgesTable)[*i][*j] = (edgesTable)[*i][v.osm_id()] + (edgesTable)[v.osm_id()][*j];
            (edgesTable)[*j][*i] = (edgesTable)[*i][v.osm_id()] + (edgesTable)[v.osm_id()][*j];
            //TODO dodac droge do bazy
//            newWays[]
        }
    }
    //Restore connections
    for(std::pair<int64_t, Edge> elem : savedEdges)
    {
        edgesTable[elem.first][v.osm_id()] = elem.second;
        edgesTable[v.osm_id()][elem.first] = elem.second;
    }



}

void contract(EdgesTable& edgesTable, const Nodes& nodes, std::map<int64_t, osm2pgr::Way>& newWays)
{
    //zakłada że nodes są w rosnącej kolejności po order
    for(unsigned int i = 0; i < nodes.size(); ++i)
    {
//        std::cout << "Jeszcze " << nodes.size() - i << std::endl;
        contractNode(edgesTable,nodes[i], nodes, newWays);
    }
}

}
