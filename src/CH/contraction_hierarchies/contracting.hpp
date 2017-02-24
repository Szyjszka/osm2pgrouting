#ifndef CONTRACTING_HPP
#define CONTRACTING_HPP

#include "../ch_types.h"

namespace RouterCH
{

void contract(EdgesTable& edgesTable, const Nodes& nodes,
              const std::map<int64_t, osm2pgr::Way>& oldWays, std::map<int64_t, osm2pgr::Way>& newWays, int64_t &firstID);

}
#endif // CONTRACTING_HPP
