#include "ordering.hpp"
#include "contracting.hpp"
#include "../algorithm_time_measure.hpp"

namespace RouterCH
{

static int32_t getNumOfWays(EdgesTable& edgesTable, const Node& v)
{
    return static_cast<int32_t>(edgesTable[v.id].size());
}


static int32_t getNumOfShortcuts(EdgesTable& edgesTable, const Node& v, Nodes &nodes,
                           ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable, const uint32_t starting_order)
{

    const int32_t actualOrder = nodes[v.id].order;
    nodes[v.id].order = std::max<int>(0, starting_order-1);
    ShorctutsInfoTable unused;
    const int32_t orderPoints = contractNode(edgesTable, nodes[v.id], nodes, shorctcutsTable, neighboursTable, false, unused);
    nodes[v.id].order = actualOrder;
    return orderPoints;
}

static int32_t getNumOfAlreadyContractedNeighbours(const Node& v, const Nodes &nodes,
                const NeighboursTable& neighboursTable, const uint32_t starting_order)
{
    int32_t orderPoints = 0;
    for(auto neighbour : neighboursTable[v.id])
    {
        if(nodes[neighbour].order < starting_order)
        {
            ++orderPoints;
        }
    }
    return orderPoints;
}

static int32_t getVoronaiRegion(const EdgesTable& edgesTable, const Node& v,
                const NeighboursTable& neighboursTable)
{
    int32_t orderPoints = 0;
    for(auto neighbour : neighboursTable[v.id])
    {
        auto closestNode = neighbour;
        auto closestDistance = std::numeric_limits<double>::max();
        for(auto edge : edgesTable.at(neighbour))
        {
            if(edge.second < closestDistance)
            {
                closestDistance = edge.second;
                closestNode = edge.first;
            }
        }
        if(closestNode == v.id)
        {
            ++orderPoints;
        }
    }
    return orderPoints;
}

static int32_t getTimeOfContraction(EdgesTable& edgesTable, const Node& v, Nodes &nodes,
                           ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable, const uint32_t starting_order)
{
    static const double TIME_MULTIPLIER = 1e8;
    const int32_t actualOrder = nodes[v.id].order;
    nodes[v.id].order = std::max<int>(0, starting_order-1);
    ShorctutsInfoTable unused;
    AlgorithmTimeMeasure atm;
    atm.startMeasurement();
    contractNode(edgesTable, nodes[v.id], nodes, shorctcutsTable, neighboursTable, false, unused);
    atm.stopMeasurement();
    const int32_t orderPoints = static_cast<int32_t>(std::ceil(atm.getMeanTime()*TIME_MULTIPLIER));
    nodes[v.id].order = actualOrder;
    return orderPoints;
}

static int32_t getGeoPoints(const Node& v, Nodes &nodes)
{
    //TODO better
    return (static_cast<int32_t>(nodes[v.id].lat * 1e6) + static_cast<int32_t>(nodes[v.id].lat * 1e6));
}

//TODO To powinno byc shortcuts - edges DELETED
static int32_t getEdgeDifference(EdgesTable& edgesTable, const Node& v, Nodes &nodes,
                           ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable, uint32_t starting_order)
{
    return getNumOfShortcuts(edgesTable, nodes[v.id], nodes, shorctcutsTable, neighboursTable, starting_order) -
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
                        ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable, const uint32_t actualIter)
{
    switch (criterium) {
    case OrderCriterium::Ways:
        return getNumOfWays(edgesTable, v);
        break;
    case OrderCriterium::Shortcuts:
        return getNumOfShortcuts(edgesTable, nodes[v.id], nodes, shorctcutsTable, neighboursTable, actualIter);
        break;
    case OrderCriterium::Ways_Plus_Shortcuts:
        return (getNumOfWays(edgesTable, v) + getNumOfShortcuts(edgesTable, nodes[v.id], nodes, shorctcutsTable, neighboursTable, actualIter));
        break;
    case OrderCriterium::EdgeDifference:
        return getEdgeDifference(edgesTable, nodes[v.id], nodes, shorctcutsTable, neighboursTable, actualIter);
        break;
    case OrderCriterium::TimeOfContraction:
        return getTimeOfContraction(edgesTable, nodes[v.id], nodes, shorctcutsTable, neighboursTable, actualIter);
        break;
    case OrderCriterium::VoronoiRegion:
        return getVoronaiRegion(edgesTable, v, neighboursTable);
        break;
    case OrderCriterium::ContractedNeighbours:
        return getNumOfAlreadyContractedNeighbours(v, nodes, neighboursTable, actualIter);
        break;
    case OrderCriterium::Geo:
        return getGeoPoints(v, nodes);
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
                    shorctcutsTable, neighboursTable, start);
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
                shorctcutsTable, neighboursTable, start);
    }
    applyOrderPoints(nodes, order, start, static_cast<uint32_t>(order.size()));
}

void orderNodes(OrderCriterium criterium, Nodes& nodes, OrderQue& orderQue, EdgesTable &edgesTable,
                ShorctutsTable& shorctcutsTable, NeighboursTable& neighboursTable)
{
    for(size_t i = 0; i < nodes.size(); ++i)
    {
        nodes[i].orderPoints = getOrderPoints(criterium, edgesTable, nodes[i], nodes,
                shorctcutsTable, neighboursTable, 0);
        orderQue.push(std::make_pair((nodes)[i].orderPoints, i));
        nodes[i].order = std::numeric_limits<uint32_t>::max(); //order in this method is given when returning
    }
}

OrderCriterium getOrderCriteriumFromString(const std::string &string)
{
    if(string == "Ways")
        return OrderCriterium::Ways;
    if(string == "Shortcuts")
        return OrderCriterium::Shortcuts;
    if(string == "Ways_Plus_Shortcuts")
        return OrderCriterium::Ways_Plus_Shortcuts;
    if(string == "EdgeDifference")
        return OrderCriterium::EdgeDifference;
    if(string == "ContractedNeighbours")
        return OrderCriterium::ContractedNeighbours;
    if(string == "VoronoiRegion")
        return OrderCriterium::VoronoiRegion;
    if(string == "TimeOfContraction")
        return OrderCriterium::TimeOfContraction;
    if(string == "Geo")
        return OrderCriterium::Geo;
    assert(false);
}

}
