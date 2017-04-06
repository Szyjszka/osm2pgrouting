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

struct Neighbour{
    Neighbour(uint32_t ID_ = std::numeric_limits<uint32_t>::max(),
              double cost_ = std::numeric_limits<double>::max())
        : id(ID_)
        , cost(cost_) {}
    bool operator == (uint32_t ID) const{
        return id == ID;
    }

    double getCost() const{
        return cost;
    }

    void setCost(double newCost){
        cost = newCost;
    }

    uint32_t id;
    double cost;
};

struct Neighbours : std::vector<Neighbour>{
public:
    Neighbour operator [] (const uint32_t id) const{
        if(std::find(begin(), end(), id) == end()){
            return Neighbour();
        }
        else
            return std::vector<Neighbour>::operator [](id);
    }
//    void push_back(const Neighbour& neighbour)
//    {
//        if(std::find(begin(), end(), neighbour.id) == end()){
//            std::vector<Neighbour>::push_back(neighbour);
//        }
//        else{
//            std::vector<Neighbour>::operator [](neighbour.id) = neighbour;
//        }
//    }
};

typedef std::vector<double> Edges;
typedef std::vector<Edges> EdgesTable;
typedef std::vector<std::vector<uint32_t> > Shortcuts;
//typedef std::vector<Neighbour> Neighbours;
typedef std::vector<Neighbours> NeighboursTable;
typedef std::vector<Shortcuts> ShorctutsTable;
typedef struct{uint32_t id; uint32_t order; uint32_t numOfWays;} Node;
typedef std::vector<Node> Nodes;
typedef std::vector<uint32_t> Order;
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
