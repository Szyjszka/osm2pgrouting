#ifndef DATA_CONVERTER_HPP
#define DATA_CONVERTER_HPP

#include "ch_types.h"
#include "Way.h"
#include "OSMDocument.h"
#include "osm_tag.h"

namespace RouterCH {

class DataConverter
{
public:
    DataConverter(osm2pgr::OSMDocument& document);

    typedef std::vector<std::vector<osm2pgr::Node *> > SplittedWays;
    typedef std::vector<osm2pgr::Way> Osm2pgrWays;
    typedef std::vector<osm2pgr::Node> Osm2pgrNodes;
private:
    double getWayCost(const std::vector<osm2pgr::Node*> &nodes) const;
    Endpoints getEntpoints(const std::vector<osm2pgr::Node*>& way) const;
    void getTagForNewWays(const osm2pgr::OSMDocument& document);

    Osm2pgrWays createNewWays();
    void upgradeWays(osm2pgr::OSMDocument& document);
    void convertToInternalFormat(const osm2pgr::OSMDocument& document);
    std::map<int64_t, uint32_t> getNumberOfWaysFromNode(const SplittedWays& splittedWays);
    SplittedWays createSplittedWays(const osm2pgr::OSMDocument &document);

    int64_t nextWayID;
    RouterCH::EdgesTable edgesTable;
    SplittedWays splittedWays;
    RouterCH::Nodes nodes;
    ShorctutsTable shortcutsTable;
    Osm2pgrNodes osm2pgrNodes;
    RouterCH::Nodes nodesWithRoads;
    std::map<int64_t, uint32_t> IDconverter;
    std::map<uint32_t, int64_t> IDconverterBack;
    Order order;
    osm2pgr::Tag tagForNewWays;
};

}
#endif // DATA_CONVERTER_HPP
