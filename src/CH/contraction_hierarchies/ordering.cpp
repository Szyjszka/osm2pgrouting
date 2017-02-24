#include "ordering.hpp"

//using namespace RouterCH;
namespace RouterCH
{
void simple_order(Nodes* nodes)
{
    for(unsigned int i = 0; i <nodes->size(); ++i)
    {
        (*nodes)[i].order = i;
    }

    //W zasadzie niepotrzebne w tym wypadku :) ale zeby potem nei zapomnieć
    //    std::sort(nodes->begin(), nodes->end());
}

void number_of_way_order(Nodes *nodes, const EdgesTable &edgesTable)
{
    std::map<int64_t, unsigned int> numberOfWaysFromNode;
    for(unsigned int i = 0; i <nodes->size(); ++i)
    {
        numberOfWaysFromNode[(*nodes)[i].osm_id()] = edgesTable.at((*nodes)[i].osm_id()).size();
    }
    std::sort(nodes->begin(), nodes->end(),
              [&numberOfWaysFromNode](Node a, Node b)->bool {return numberOfWaysFromNode[a.osm_id()] < numberOfWaysFromNode[b.osm_id()];});
    for(unsigned int i = 0; i <nodes->size(); ++i)
    {
        (*nodes)[i].order = i;
    }
}

}
