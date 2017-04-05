#include "ordering.hpp"
#include "contracting.hpp"

//using namespace RouterCH;
namespace RouterCH
{

//TODO can be better - we've got neighbours table
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

void simple_order(Nodes* nodes, Order *order)
{
    for(uint32_t i = 0; i <nodes->size(); ++i)
    {
        (*order)[i] = (*nodes)[i].id;
        (*nodes)[i].order = i;
    }

    //W zasadzie niepotrzebne w tym wypadku :) ale zeby potem nei zapomnieć
    //    std::sort(nodes->begin(), nodes->end());
}

void order_with_num_of_roads(Nodes *nodes, Order *order)
{
    Nodes copyOfNodes(*nodes);
//    std::copy(nodes->begin(), nodes->end(), copyOfNodes.begin());
    std::sort(copyOfNodes.begin(), copyOfNodes.end(), [](Node a, Node b) {return a.numOfWays < b.numOfWays;});
    for(uint32_t i = 0; i <copyOfNodes.size(); ++i)
    {
        (*order)[i] = copyOfNodes[i].id;
        (*nodes)[copyOfNodes[i].id].order = i;
    }
}

void order_with_num_of_roads(Nodes *nodes, Order *order, const EdgesTable &edgesTable, const uint32_t start)
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

    for(uint32_t i = start; i <nodes->size(); ++i)
    {
        (*order)[i] = copyOfNodes[i-start].id;
        assert((*nodes)[copyOfNodes[i-start].id].order >= start);
        (*nodes)[copyOfNodes[i-start].id].order = i;
    }
}

void order_with_number_of_shorctuts(Nodes *nodes, Order *order, EdgesTable &edgesTable, const uint32_t start, ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable)
{
    Nodes copyOfNodes;
    for(size_t i = start; i < order->size(); ++i)
    {
        assert((*nodes)[(*order)[i]].order >= start);
        (*nodes)[(*order)[i]].order = INF;
        (*nodes)[(*order)[i]].numOfWays = contractNode(edgesTable, (*nodes)[(*order)[i]], *nodes, shorctcutsTable, neighboursTable, false);
        copyOfNodes.push_back((*nodes)[(*order)[i]]);
    }
    std::sort(copyOfNodes.begin(), copyOfNodes.end(), [](Node a, Node b) {return a.numOfWays < b.numOfWays;});

    for(uint32_t i = start; i <nodes->size(); ++i)
    {
        (*order)[i] = copyOfNodes[i-start].id;
        assert((*nodes)[copyOfNodes[i-start].id].order >= start);
        (*nodes)[copyOfNodes[i-start].id].order = i;
    }
}

}
