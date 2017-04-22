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

uint32_t OrderSupervisor::getIndexOfNextNode() const
{
    return actualNode;
}

void OrderSupervisor::updateOrder(Nodes &nodes, EdgesTable &edgesTable, NeighboursTable &neighboursTable, ShorctutsTable &shortcutsTable)
{
    if(strategy != Strategy::LazyUpdate)
    {
        actualNode = orderTable[counter];
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
        actualNode = orderTable[counter];
    }
    else
    {
        bool notBestSolution = true;
        OrderElem orderElem;
        do
        {
            assert(orderQue.size());
            orderElem = orderQue.top();
            orderQue.pop();
            orderElem.first = getOrderPoints(orderCriterium, edgesTable, nodes[orderElem.second],
                    nodes, shortcutsTable, neighboursTable);
            notBestSolution = orderElem.first > orderQue.top().first;
            if(notBestSolution)
                orderQue.push(orderElem);
        }while(notBestSolution);
        actualNode = orderElem.second;
        nodes[actualNode].order = counter;
    }
    ++counter;
}

}
