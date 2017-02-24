#ifndef CONTRACTING_HPP
#define CONTRACTING_HPP

#include "../ch_types.h"

namespace RouterCH
{

void contract(EdgesTable& edgesTable, const Nodes& nodes, std::map<int64_t, osm2pgr::Way>& newWays);

}
#endif // CONTRACTING_HPP
