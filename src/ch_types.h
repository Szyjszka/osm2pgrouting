#ifndef CH_TYPES_H
#define CH_TYPES_H

#include <vector>
#include "Node.h"

namespace osm2pgr {

typedef std::vector<unsigned int> Edges;
typedef std::vector<Edges> EdgesTable;
typedef std::vector<std::vector<std::vector<unsigned int> > > ShorctutsTable;
typedef std::vector<Node> Nodes;

struct Endpoints{
    Node start, end;
};

static const unsigned int INF = UINT_MAX;
static const signed int NO_PRECCESSOR = -1;
}
#endif // CH_TYPES_H
