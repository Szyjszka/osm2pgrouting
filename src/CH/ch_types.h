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

struct Edge{
    int64_t way_id;
    double cost;

    operator double() const {return cost;}
    double operator =(double d) {cost = d; return cost;}
};

typedef osm2pgr::Node Node;
typedef std::map<int64_t, Edge> Edges;
typedef std::map<int64_t, Edges> EdgesTable;
typedef std::vector<std::vector<std::vector<double> > > ShorctutsTable;
typedef std::vector<osm2pgr::Node> Nodes;
typedef struct { Nodes nodes; double cost; int64_t id;} Route;
typedef std::vector<Route> Graph;

static const unsigned int INF = UINT_MAX;
static const signed int NO_PRECCESSOR = -1;
}
#endif // CH_TYPES_H
