#ifndef CH_TYPES_H
#define CH_TYPES_H

#include <map>
#include <vector>
#include "Node.h"

namespace osm2pgr {

struct Endpoints{
    Node start, end;
};

struct Edge{
    int64_t end_id;
    int64_t way_id;
    double cost;

    operator double() const {return cost;}
};

typedef std::vector<Edge> Edges;
typedef std::map<int64_t, Edges> EdgesTable;
//typedef std::vector<Edges> EdgesTable;
typedef std::vector<std::vector<std::vector<unsigned int> > > ShorctutsTable;
typedef std::vector<Node> Nodes;

static const unsigned int INF = UINT_MAX;
static const signed int NO_PRECCESSOR = -1;
}
#endif // CH_TYPES_H
