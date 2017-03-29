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
private:
    double getWayCost(const std::vector<osm2pgr::Node*> &nodes) const;
    Endpoints getEntpoints(const std::vector<osm2pgr::Node*>& way) const;
    void getTagForNewWays(const osm2pgr::OSMDocument& document);

    std::vector<osm2pgr::Way> createNewWays();
    void upgradeWays(osm2pgr::OSMDocument& document);
    void convertToInternalFormat(const osm2pgr::OSMDocument& document);
    SplittedWays createSplittedWays(const osm2pgr::OSMDocument &document);
    int64_t nextWayID;
    RouterCH::EdgesTable edgesTable;
    SplittedWays splittedWays;
    RouterCH::Nodes nodes;
    ShorctutsTable shortcutsTable;
    std::vector<osm2pgr::Node> osm2pgrNodes;
    std::vector<Node> nodesWithRoads;
    std::map<int64_t, uint32_t> IDconverter;
    std::map<uint32_t, int64_t> IDconverterBack;
    std::vector<uint32_t> order;
    osm2pgr::Tag tagForNewWays;

    //    int64_t id_table[] = { 451784587, 451784588, 451784589, 451784590, 451784591, 451784592};
};

}
#endif // DATA_CONVERTER_HPP
