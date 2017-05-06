#ifndef DATA_CONVERTER_HPP
#define DATA_CONVERTER_HPP

#include "ch_types.h"
#include "Way.h"
#include "contraction_hierarchies/ordering.hpp"
#include "contraction_hierarchies/ordersupervisor.hpp"
#include "OSMDocument.h"
#include "osm_tag.h"

namespace RouterCH {

class DataConverter
{
public:
    DataConverter(osm2pgr::OSMDocument& document, OrderCriterium orderCriterium,
                  OrderSupervisor::Strategy strategy, const std::string& measureFileName);

    typedef std::vector<std::vector<osm2pgr::Node *> > SplittedWays;
    typedef std::vector<osm2pgr::Way> Osm2pgrWays;
    typedef std::vector<osm2pgr::Node> Osm2pgrNodes;
    typedef std::map<int64_t, uint32_t> NumberOfWaysFromNode;
private:
    double getWayCost(const std::vector<osm2pgr::Node*> &nodes) const;
    Endpoints getEntpoints(const std::vector<osm2pgr::Node*>& way) const;
    osm2pgr::Tag getTagForNewWays(const osm2pgr::OSMDocument& document);

    Osm2pgrWays createNewWays(const osm2pgr::OSMDocument& document);
    void upgradeWays(osm2pgr::OSMDocument& document);
    void convertToInternalFormat(const osm2pgr::OSMDocument& document);
    NumberOfWaysFromNode getNumberOfWaysFromNode(const SplittedWays& splittedWays);
    SplittedWays createSplittedWays(const osm2pgr::OSMDocument &document);
    void groupNodesWithRoads(const NumberOfWaysFromNode& numberOfWaysFromNode, const osm2pgr::OSMDocument &document);
    void fillEdgesTable(const SplittedWays& splittedWays, const size_t numberOfWays);

    int64_t nextWayID;
    RouterCH::EdgesTable edgesTable;
    SplittedWays splittedWays;
    RouterCH::Nodes nodes;
    ShorctutsTable shortcutsTable;
    Osm2pgrNodes osm2pgrNodes;
    NeighboursTable neighboursTable;
    RouterCH::Nodes nodesWithRoads;
    ShorctutsInfoTable shortcutInfos;
    std::map<int64_t, uint32_t> IDconverter;
};

}
#endif // DATA_CONVERTER_HPP
