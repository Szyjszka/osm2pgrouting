#include "ordering.hpp"
#include "contracting.hpp"

namespace RouterCH
{

static int32_t getNumOfWays(EdgesTable& edgesTable, const Node& v)
{
    return static_cast<int32_t>(edgesTable[v.id].size());
}


static int32_t getNumOfShortcuts(EdgesTable& edgesTable, const Node& v, Nodes &nodes,
                           ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable)
{

    const int32_t actualOrder = nodes[v.id].order;
    nodes[v.id].order = 0;
    ShorctutsInfoTable unused;
    const int32_t orderPoints = contractNode(edgesTable, nodes[v.id], nodes, shorctcutsTable, neighboursTable, false, unused);
    nodes[v.id].order = actualOrder;
    return orderPoints;
}

static int32_t getEdgeDifference(EdgesTable& edgesTable, const Node& v, Nodes &nodes,
                           ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable)
{
    return getNumOfShortcuts(edgesTable, nodes[v.id], nodes, shorctcutsTable, neighboursTable) -
           static_cast<int32_t>(getNumOfWays(edgesTable,  nodes[v.id]));
}

void simple_order(Nodes* nodes, Order *order)
{
    for(uint32_t i = 0; i <nodes->size(); ++i)
    {
        (*order)[i] = (*nodes)[i].id;
        (*nodes)[i].order = i;
    }
}

int32_t getOrderPoints(OrderCriterium criterium, EdgesTable& edgesTable, const Node& v, Nodes &nodes,
                        ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable)
{
    switch (criterium) {
    case OrderCriterium::Ways:
        return getNumOfWays(edgesTable, v);
        break;
    case OrderCriterium::Shortcuts:
        return getNumOfShortcuts(edgesTable, nodes[v.id], nodes, shorctcutsTable, neighboursTable);
        break;
    case OrderCriterium::EdgeDifference:
        return getEdgeDifference(edgesTable, nodes[v.id], nodes, shorctcutsTable, neighboursTable);
        break;
    default:
        break;
    }
    assert(false);
    return 0;
}

void applyOrderPoints(Nodes& nodes, Order& order, const uint32_t start, const uint32_t end)
{
    std::sort(order.begin() + start, order.begin() + end, [nodes](uint32_t a, uint32_t b)
        {return nodes[a].orderPoints < nodes[b].orderPoints;});
    for(unsigned int i = start; i < order.size(); ++i)
    {
        nodes[order[i]].order = i;
    }
}

void updateNeighbours(OrderCriterium criterium, Nodes& nodes, Order& order, EdgesTable &edgesTable, const uint32_t start,
                ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable, const Neighbours& nodesThatChanged)
{
    if(!nodesThatChanged.size())
    {
        return;
    }

    uint32_t end = 0;
    for(size_t i = 0; i < nodesThatChanged.size(); ++i)
    {
        if(nodes[nodesThatChanged[i]].order >= start)
        {
            nodes[nodesThatChanged[i]].orderPoints = getOrderPoints(criterium, edgesTable, nodes[nodesThatChanged[i]], nodes,
                    shorctcutsTable, neighboursTable);
            end = std::max(nodes[nodesThatChanged[i]].order, end);
        }
    }
    if(end > start)
    {
        applyOrderPoints(nodes, order, start, end);
    }
}

void orderNodes(OrderCriterium criterium, Nodes& nodes, Order& order, EdgesTable &edgesTable, const uint32_t start,
                ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable)
{
    for(size_t i = start; i < order.size(); ++i)
    {
        assert((nodes)[order[i]].order >= start);
        (nodes)[order[i]].orderPoints = getOrderPoints(criterium, edgesTable, nodes[order[i]], nodes,
                shorctcutsTable, neighboursTable);
    }
    applyOrderPoints(nodes, order, start, order.size());
}

void orderNodes(OrderCriterium criterium, Nodes& nodes, OrderQue& orderQue, EdgesTable &edgesTable,
                ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable)
{
    for(size_t i = 0; i < nodes.size(); ++i)
    {
        nodes[i].orderPoints = getOrderPoints(criterium, edgesTable, nodes[i], nodes,
                shorctcutsTable, neighboursTable);
        orderQue.push(std::make_pair((nodes)[i].orderPoints, i));
        nodes[i].order = std::numeric_limits<uint32_t>::max(); //order in this method is given when returning
    }
}

}
