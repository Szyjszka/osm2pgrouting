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

struct QPoint{
    unsigned x;
    unsigned y;

};


typedef std::vector<std::vector<double> > EdgesTable;
typedef std::vector<std::vector<std::vector<unsigned int> > > ShorctutsTable;
typedef struct{QPoint coord; unsigned int id; unsigned int order; unsigned int numOfWays;} Node;
typedef std::vector<Node> Nodes;
typedef struct { Nodes nodes; double cost; unsigned int id;} Route;

static const unsigned int INF = UINT_MAX;
static const signed int NO_PRECCESSOR = -1;
}
#endif // CH_TYPES_H
