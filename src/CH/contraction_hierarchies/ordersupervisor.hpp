#ifndef ORDERSUPERVISOR_HPP
#define ORDERSUPERVISOR_HPP

#include "CH/ch_types.h"
#include "ordering.hpp"
#include "../distancemanager.hpp"
#include "memory"

namespace RouterCH
{

class OrderSupervisor
{
public:
    enum class Strategy
    {
        UpdateEveryRound,
        UpdateEvery10Round,
        UpdateEvery50Round,
        UpdateEvery100Round,
        LazyUpdate,
        UpdateNeighbours
    };

    static Strategy getOrderStrategyFromString(const std::string& string);
    OrderSupervisor(const Strategy strategy_, const OrderCriterium orderCriterium_, Nodes& nodes,
                    EdgesTable& edgesTable, NeighboursTable& neighboursTable, ShorctutsTable& shortcutsTable, const OrderParameters& OrderParameters);
    uint32_t getIndexOfNextNode() const;
    void updateOrder(Nodes& nodes, EdgesTable& edgesTable, NeighboursTable& neighbours, ShorctutsTable& shortcutsTable);
    uint32_t getSettledNodesLimit() const;
    uint32_t getHopLimit() const;
private:
    Strategy strategy;
    OrderCriterium orderCriterium;
    OrderQue orderQue;
    Order orderTable;
    uint32_t actualNode;
    uint32_t counter;
    OrderParameters orderParameters;
    uint32_t settledNodesLimit;
    uint32_t hopLimit;
    std::unique_ptr<DistanceManager> distanceManager;
};


}
#endif // ORDERSUPERVISOR_HPP
