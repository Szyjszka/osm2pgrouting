#ifndef CONTRACTING_HPP
#define CONTRACTING_HPP

#include "../ch_types.h"

namespace RouterCH
{

void contract(EdgesTable& edgesTable, const Nodes& nodes, ShorctutsTable& shortcutsTable,
              const std::vector<unsigned int> &order, const std::map<uint32_t, int64_t>& IDconverterBack);

}
#endif // CONTRACTING_HPP
