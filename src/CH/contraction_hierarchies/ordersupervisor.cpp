#include "ordersupervisor.hpp"

namespace RouterCH
{

OrderSupervisor::OrderSupervisor(const OrderSupervisor::Strategy strategy_, const OrderCriterium orderCriterium_,
                                 Nodes &nodes, EdgesTable &edgesTable, NeighboursTable &neighboursTable, ShorctutsTable &shortcutsTable)
    : strategy(strategy_)
    , orderCriterium(orderCriterium_)
    , counter(0)
{
    if(strategy != Strategy::LazyUpdate)
    {
        orderTable.resize(nodes.size());
        simple_order(&nodes, &orderTable);
        orderNodes(orderCriterium, nodes, orderTable, edgesTable, 0, shortcutsTable, neighboursTable);
    }
    else
    {
        orderNodes(orderCriterium, nodes, orderQue, edgesTable, shortcutsTable, neighboursTable);
    }

}

uint32_t OrderSupervisor::getIndexOfNextNode()
{
    if(strategy != Strategy::LazyUpdate)
    {
        assert(counter < orderTable.size());
        actualNode = orderTable[counter];
    }
    else
    {
        assert(orderQue.size());
        actualNode = orderQue.top().second;
        orderQue.pop();
    }
    ++counter;
    return actualNode;
}

void OrderSupervisor::updateOrder(Nodes &nodes, EdgesTable &edgesTable, NeighboursTable &neighboursTable, ShorctutsTable &shortcutsTable)
{
    if(strategy != Strategy::LazyUpdate)
    {
        if(strategy == Strategy::UpdateEveryRound)
        {
            orderNodes(orderCriterium, nodes, orderTable, edgesTable, counter,
                       shortcutsTable, neighboursTable);
        }
        else if(strategy == Strategy::UpdateNeighbours)
        {
            updateNeighbours(orderCriterium, nodes, orderTable, edgesTable, counter,
                             shortcutsTable, neighboursTable, neighboursTable[nodes[actualNode].id]);
        }
    }
    else
    {
        OrderElem orderElem = orderQue.top();
    }
}

}
