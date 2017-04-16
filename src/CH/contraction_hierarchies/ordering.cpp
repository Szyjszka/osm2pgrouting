#include "ordering.hpp"
#include "contracting.hpp"

//using namespace RouterCH;
namespace RouterCH
{

static uint32_t getNumOfWays(EdgesTable& edgesTable, const Node& v)
{
    return edgesTable[v.id].size();
}

//static uint32_t getEdgeDifference(EdgesTable& edgesTable, const Node& v, const Nodes &nodes,
//                           ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable)
//{

//}


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

uint32_t getOrderPoints(OrderCriterium criterium, EdgesTable& edgesTable, const Node& v, Nodes &nodes,
                        ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable)
{
        if(criterium == OrderCriterium::Ways)
        {
            return getNumOfWays(edgesTable, v);
        }
        else if(criterium == OrderCriterium::Shortcuts)
        {
            const uint32_t actualOrder = nodes[v.id].order;
            nodes[v.id].order = 0;
            const uint32_t orderPoints = contractNode(edgesTable, nodes[v.id], nodes, shorctcutsTable, neighboursTable, false);
            nodes[v.id].order = actualOrder;
            return orderPoints;
        }
        assert(false);
        return 0;
}


void orderNodes(OrderCriterium criterium, Nodes& nodes, Order& order, EdgesTable &edgesTable, const uint32_t start,
                ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable)
{
    Nodes copyOfNodes;
    for(size_t i = start; i < order.size(); ++i)
    {
        assert((nodes)[(order)[i]].order >= start);
        (nodes)[(order)[i]].pointsForOrder = getOrderPoints(criterium, edgesTable, nodes[order[i]], nodes,
                shorctcutsTable, neighboursTable);
        copyOfNodes.push_back(nodes[order[i]]);
    }
    std::sort(copyOfNodes.begin(), copyOfNodes.end(), [](Node a, Node b) {return a.pointsForOrder < b.pointsForOrder;});

    for(uint32_t i = start; i <nodes.size(); ++i)
    {
        (order)[i] = copyOfNodes[i-start].id;
        assert(nodes[copyOfNodes[i-start].id].order >= start);
        (nodes)[copyOfNodes[i-start].id].order = i;
    }
}

}
