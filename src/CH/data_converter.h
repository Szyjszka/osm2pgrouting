#ifndef DATA_CONVERTER_HPP
#define DATA_CONVERTER_HPP

#include "ch_types.h"
#include "Way.h"
#include "OSMDocument.h"

namespace osm2pgr {

class DataConverter
{
public:
    DataConverter(const OSMDocument& document);
private:
    double getWayCost(const Way& way) const;
    Endpoints getEntpoints(const Way& way) const;

    EdgesTable edgesTable;
    Nodes nodes;
};

}
#endif // DATA_CONVERTER_HPP
