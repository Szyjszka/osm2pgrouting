#ifndef ORDERSUPERVISOR_HPP
#define ORDERSUPERVISOR_HPP

#include "CH/ch_types.h"
#include "ordering.hpp"

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

    OrderSupervisor(const Strategy strategy_, const OrderCriterium orderCriterium_, Nodes& nodes,
                    EdgesTable& edgesTable, NeighboursTable& neighboursTable, ShorctutsTable& shortcutsTable);
    uint32_t getIndexOfNextNode();
    void updateOrder(Nodes& nodes, EdgesTable& edgesTable, NeighboursTable& neighbours, ShorctutsTable& shortcutsTable);
private:
    Strategy strategy;
    OrderCriterium orderCriterium;
    OrderQue orderQue;
    Order orderTable;
    uint32_t actualNode;
    uint32_t counter;
};


}
#endif // ORDERSUPERVISOR_HPP
