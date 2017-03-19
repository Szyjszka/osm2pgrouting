#include "ordering.hpp"

//using namespace RouterCH;
namespace RouterCH
{

uint32_t getNumOfWays(const uint32_t nodeID, const EdgesTable& edgesTable)
{
    uint32_t i = 0;
    for(auto edge : edgesTable[nodeID])
    {
        if(edge < INF)
            ++i;
    }
    return i;
}

void simple_order(Nodes* nodes, std::vector<unsigned int>* order)
{
    for(unsigned int i = 0; i <nodes->size(); ++i)
    {
        (*order)[i] = (*nodes)[i].id;
        (*nodes)[i].order = i;
    }

    //W zasadzie niepotrzebne w tym wypadku :) ale zeby potem nei zapomnieć
    //    std::sort(nodes->begin(), nodes->end());
}

void order_with_num_of_roads(Nodes *nodes, std::vector<unsigned int>* order)
{
    Nodes copyOfNodes(*nodes);
//    std::copy(nodes->begin(), nodes->end(), copyOfNodes.begin());
    std::sort(copyOfNodes.begin(), copyOfNodes.end(), [](Node a, Node b) {return a.numOfWays < b.numOfWays;});
    for(unsigned int i = 0; i <copyOfNodes.size(); ++i)
    {
        (*order)[i] = copyOfNodes[i].id;
        (*nodes)[copyOfNodes[i].id].order = i;
    }
}

void order_with_num_of_roads(Nodes *nodes, std::vector<unsigned int>* order, const EdgesTable &edgesTable, const int start)
{
    Nodes copyOfNodes;
    for(size_t i = start; i < order->size(); ++i)
    {
        assert((*nodes)[(*order)[i]].order >= start);
        (*nodes)[(*order)[i]].order = INF;
        (*nodes)[(*order)[i]].numOfWays = getNumOfWays((*nodes)[(*order)[i]].id, edgesTable);
        copyOfNodes.push_back((*nodes)[(*order)[i]]);
    }
    std::sort(copyOfNodes.begin(), copyOfNodes.end(), [](Node a, Node b) {return a.numOfWays < b.numOfWays;});

    for(unsigned int i = start; i <nodes->size(); ++i)
    {
        (*order)[i] = copyOfNodes[i-start].id;
        assert((*nodes)[copyOfNodes[i-start].id].order >= start);
        (*nodes)[copyOfNodes[i-start].id].order = i;
    }
}



unsigned int tryToContractNode(const EdgesTable& edgesTable, const Node& v, const Nodes &nodes)
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
            if(((edgesTable)[uID][v.id] < UINT_MAX) && (edgesTable)[v.id][wID] < UINT_MAX && (edgesTable)[uID][wID]>=UINT_MAX)
            {
                //jeśli (u,v,w) jest unikalną najkrótszą ścieżką
                if(chechIfShortcudNeeded(edgesTable, nodes[uID], v, nodes[wID], nodes))
                {
                    ++numberOfShortcutsCreated;
                }
            }
        }
    }
    return numberOfShortcutsCreated;

}

void order_with_number_of_shorctuts(Nodes *nodes, std::vector<unsigned int> *order, const EdgesTable &edgesTable, const int start)
{
    Nodes copyOfNodes;
    for(size_t i = start; i < order->size(); ++i)
    {
        assert((*nodes)[(*order)[i]].order >= start);
        (*nodes)[(*order)[i]].order = INF;
        (*nodes)[(*order)[i]].numOfWays = tryToContractNode(edgesTable, (*nodes)[(*order)[i]], *nodes);
        copyOfNodes.push_back((*nodes)[(*order)[i]]);
    }
    std::sort(copyOfNodes.begin(), copyOfNodes.end(), [](Node a, Node b) {return a.numOfWays < b.numOfWays;});

    for(unsigned int i = start; i <nodes->size(); ++i)
    {
        (*order)[i] = copyOfNodes[i-start].id;
        assert((*nodes)[copyOfNodes[i-start].id].order >= start);
        (*nodes)[copyOfNodes[i-start].id].order = i;
    }
}

}
