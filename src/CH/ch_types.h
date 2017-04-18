#ifndef CH_TYPES_H
#define CH_TYPES_H

#include <map>
#include <vector>
#include <queue>
#include "Node.h"
#include "Way.h"

namespace RouterCH {
struct Endpoints{
    osm2pgr::Node start, end;
};

struct Edges : std::map<uint32_t, double>
{
    double at(uint32_t key) const{
        if(find(key)==end()){
            return std::numeric_limits<double>::max();
        }
        else return std::map<uint32_t, double>::at(key);
    }
//    double operator [](uint32_t key){
//        if(find(key)==end()){
//            return std::numeric_limits<double>::max();
//        }
//        else return std::map<uint32_t, double>::operator [](key);
//    }
};
typedef std::vector<Edges> EdgesTable;
typedef std::map<uint32_t, std::vector<uint32_t> > Shortcuts;
typedef std::vector<uint32_t> Neighbours;
typedef std::vector<Neighbours> NeighboursTable;
typedef std::vector<Shortcuts> ShorctutsTable;
typedef struct{uint32_t id; uint32_t order; int32_t orderPoints;} Node;
typedef std::vector<Node> Nodes;
typedef std::vector<uint32_t> Order;
typedef std::pair<int32_t, uint32_t> OrderElem;
typedef std::priority_queue<OrderElem, std::vector<OrderElem> , std::greater<OrderElem> > OrderQue;
typedef struct { Nodes nodes; double cost; uint32_t id;} Route;

struct EdgeWithNodes{
    uint32_t A,B;
    double cost;
};

typedef std::vector<EdgeWithNodes> EdgeWithNodesTable;

static const uint32_t INF = UINT_MAX;
static const signed int NO_PRECCESSOR = -1;
}
#endif // CH_TYPES_H
