#ifndef DATA_CONVERTER_HPP
#define DATA_CONVERTER_HPP

#include "ch_types.h"
#include "Way.h"
#include "OSMDocument.h"

namespace RouterCH {

class DataConverter
{
public:
    DataConverter(const osm2pgr::OSMDocument& document);
private:
    double getWayCost(const osm2pgr::Way& way) const;
    Endpoints getEntpoints(const osm2pgr::Way& way) const;

    RouterCH::EdgesTable edgesTable;
    RouterCH::Nodes nodes;
    std::map<int64_t, osm2pgr::Way> newWays;
};

}
#endif // DATA_CONVERTER_HPP
