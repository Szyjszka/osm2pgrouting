#ifndef DATA_CONVERTER_HPP
#define DATA_CONVERTER_HPP

#include "ch_types.h"
#include "Way.h"
#include "OSMDocument.h"

namespace RouterCH {

class DataConverter
{
public:
    DataConverter(osm2pgr::OSMDocument& document);
private:
    double getWayCost(const osm2pgr::Way& way) const;
    Endpoints getEntpoints(const osm2pgr::Way& way) const;

    std::vector<osm2pgr::Way> createNewWays(const osm2pgr::OSMDocument &document);

    int64_t nextWayID;
    RouterCH::EdgesTable edgesTable;
    RouterCH::Nodes nodes;
    ShorctutsTable shortcutsTable;
    std::vector<osm2pgr::Node> osm2pgrNodes;
    std::vector<Node> nodesWithRoads;
    std::map<int64_t, osm2pgr::Way> newWays;
    std::map<int64_t, unsigned int> IDconverter;
    std::map<int64_t, unsigned int> IDconverterBack;
};

}
#endif // DATA_CONVERTER_HPP
