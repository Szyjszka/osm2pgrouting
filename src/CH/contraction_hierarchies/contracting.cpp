#include <algorithm>
#include <cassert>
#include <climits>
#include <iostream>
#include "contracting.hpp"
#include "../shortest_path_algorithms/dijkstra.hpp"

namespace RouterCH
{

bool chechIfShortcudNeeded(EdgesTable& edgesTable, const int64_t u,
                           const int64_t v, const int64_t w, const Nodes &nodes, osm2pgr::Way& newWay)
{
    Route checkedShortctut;
    checkedShortctut.cost = edgesTable[u][v] + edgesTable[v][w];

    Route sh = dijkstra(edgesTable, u, w, nodes, checkedShortctut.cost);

    //!TODO: Koszt takiej drogi wcale nie musi byc rowny sumie ich - zalezy na co pozniej bedziemy patrzec
    if( sh.cost > checkedShortctut.cost)
    {
        for(Node& node : sh.nodes)
        {
            newWay.add_node(&node);
        }
    }
    return ( sh.cost > checkedShortctut.cost);
}

void contractNode(EdgesTable& edgesTable, const Node& v, const Nodes &nodes,
                  const std::map<int64_t, osm2pgr::Way>& oldWays, std::map<int64_t, osm2pgr::Way>& newWays,
                  int64_t& firstID)
{
    // dla każdej pary (u, v) i (v,w) z krawędzi
    if(edgesTable.find(v.osm_id()) == edgesTable.end())
    {
        return;
    }

    Edges savedEdges;
    std::vector<int64_t> connectedNodes;
    //erase connections
    //TODO czy mozna usunac 2 rozne id przez to ?
    for(std::pair<int64_t, Edge> elem : edgesTable[v.osm_id()])
    {
        savedEdges[elem.first] = elem.second;
        savedEdges[elem.first].way_id = elem.second.way_id;
        edgesTable[elem.first].erase(v.osm_id());
        edgesTable[v.osm_id()].erase(elem.first);
        connectedNodes.push_back(elem.first);
        assert(elem.second.way_id != 0);
    }

    for(auto i = connectedNodes.begin(); i != prev(connectedNodes.end()); ++i)
    for(auto j = next(i); j != connectedNodes.end(); ++j)
    {

        osm2pgr::Way newWay;
        if(chechIfShortcudNeeded(edgesTable, *i , v.osm_id(), *j, nodes, newWay))
        {
        //dodaj skrót (u,v,w)
            (edgesTable)[*i][*j] = (savedEdges)[*i] + (savedEdges)[*j];
            (edgesTable)[*j][*i] = (savedEdges)[*i] + (savedEdges)[*j];
//            std::cout <<"Dodano droge o koszcie " << (edgesTable)[*i][*j]  << std::endl;
            //TODO Czy nie powinnismy brac obu way_id ??
            int64_t checkWayID = (savedEdges)[*i].way_id;
            edgesTable[*i][*j].way_id = checkWayID;
            edgesTable[*j][*i].way_id = checkWayID;
            newWay.maxspeed_forward(oldWays.at(checkWayID).maxspeed_forward());
            newWay.maxspeed_backward(oldWays.at(checkWayID).maxspeed_backward());
            newWay.setID(firstID++);
            newWays[firstID] = newWay;
        }
    }
    //Restore connections
    for(std::pair<int64_t, Edge> elem : savedEdges)
    {
        edgesTable[elem.first][v.osm_id()] = elem.second;
        edgesTable[v.osm_id()][elem.first] = elem.second;
        edgesTable[elem.first][v.osm_id()].way_id = elem.second.way_id;
        edgesTable[v.osm_id()][elem.first].way_id = elem.second.way_id;
        assert(elem.second.way_id != 0);
    }



}

void contract(EdgesTable& edgesTable, const Nodes& nodes,
              const std::map<int64_t, osm2pgr::Way>& oldWays, std::map<int64_t, osm2pgr::Way>& newWays,
              int64_t& firstID)
{
    //zakłada że nodes są w rosnącej kolejności po order
    for(unsigned int i = 0; i < nodes.size(); ++i)
    {
//        std::cout << "Jeszcze " << nodes.size() - i << std::endl;
        contractNode(edgesTable,nodes[i], nodes, oldWays, newWays, firstID);
    }
}

}
