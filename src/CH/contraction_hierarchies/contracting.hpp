#ifndef CONTRACTING_HPP
#define CONTRACTING_HPP

#include "../ch_types.h"

namespace RouterCH
{

void contract(EdgesTable& edgesTable, const Nodes& nodes, ShorctutsTable& shortcutsTable,
              const std::vector<unsigned int> &order);

}
#endif // CONTRACTING_HPP
