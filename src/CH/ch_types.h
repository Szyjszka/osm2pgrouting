#ifndef CH_TYPES_H
#define CH_TYPES_H

#include <map>
#include <vector>
#include "Node.h"
#include "Way.h"

namespace RouterCH {
struct Endpoints{
    osm2pgr::Node start, end;
};

typedef std::vector<double> Edges;
typedef std::vector<Edges> EdgesTable;
typedef std::vector<std::vector<uint32_t> > Shortcuts;
typedef std::vector<uint32_t> Neighbours;
typedef std::vector<Neighbours> NeighboursTable;
typedef std::vector<Shortcuts> ShorctutsTable;
typedef struct{uint32_t id; uint32_t order; uint32_t numOfWays;} Node;
typedef std::vector<Node> Nodes;
typedef std::vector<uint32_t> Order;
typedef struct { Nodes nodes; double cost; uint32_t id;} Route;

static const uint32_t INF = UINT_MAX;
static const signed int NO_PRECCESSOR = -1;
}
#endif // CH_TYPES_H
