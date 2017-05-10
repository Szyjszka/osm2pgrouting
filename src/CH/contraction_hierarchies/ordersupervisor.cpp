#include "ordersupervisor.hpp"

namespace RouterCH
{

OrderSupervisor::Strategy OrderSupervisor::getOrderStrategyFromString(const std::string &string)
{
    if(string == "UpdateEveryRound")
        return OrderSupervisor::Strategy::UpdateEveryRound;
    if(string == "UpdateEvery10Round")
        return OrderSupervisor::Strategy::UpdateEvery10Round;
    if(string == "UpdateEvery50Round")
        return OrderSupervisor::Strategy::UpdateEvery50Round;
    if(string == "UpdateEvery100Round")
        return OrderSupervisor::Strategy::UpdateEvery100Round;
    if(string == "LazyUpdate")
        return OrderSupervisor::Strategy::LazyUpdate;
    if(string == "UpdateNeighbours")
        return OrderSupervisor::Strategy::UpdateNeighbours;

    assert(false);
}

OrderSupervisor::OrderSupervisor(const OrderSupervisor::Strategy strategy_, const OrderCriterium orderCriterium_,
                                 Nodes &nodes, EdgesTable &edgesTable, NeighboursTable &neighboursTable, ShorctutsTable &shortcutsTable, const OrderParameters &orderParameters_)
    : strategy(strategy_)
    , orderCriterium(orderCriterium_)
    , counter(0)
    , orderParameters(orderParameters_)
    , settledNodesLimit(INF)
    , hopLimit(INF)
{
    if(strategy != Strategy::LazyUpdate)
    {
        orderTable.resize(nodes.size());
        simple_order(&nodes, &orderTable);
        orderNodes(orderCriterium, nodes, orderTable, edgesTable, 0, shortcutsTable, neighboursTable, orderParameters, hopLimit, settledNodesLimit);
    }
    else
    {
        orderNodes(orderCriterium, nodes, orderQue, edgesTable, shortcutsTable, neighboursTable, orderParameters, hopLimit, settledNodesLimit);
    }
    if(orderCriterium_ == OrderCriterium::MyAlgorithm)
    {
        hopLimit = INF;
        settledNodesLimit = INF;
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
                       shortcutsTable, neighboursTable, orderParameters, hopLimit, settledNodesLimit);
        }
        else if(strategy == Strategy::UpdateNeighbours)
        {
            updateNeighbours(orderCriterium, nodes, orderTable, edgesTable, counter,
                             shortcutsTable, neighboursTable, neighboursTable[nodes[actualNode].id], orderParameters, hopLimit, settledNodesLimit);
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
                    nodes, shortcutsTable, neighboursTable, counter, orderParameters, hopLimit, settledNodesLimit);
            notBestSolution = orderElem.first > orderQue.top().first;
            if(notBestSolution)
                orderQue.push(orderElem);
        }while(notBestSolution);
        actualNode = orderElem.second;
        nodes[actualNode].order = counter;
    }
    ++counter;
}

uint32_t OrderSupervisor::getSettledNodesLimit() const
{
    return settledNodesLimit;
}

uint32_t OrderSupervisor::getHopLimit() const
{
    return hopLimit;
}

}
