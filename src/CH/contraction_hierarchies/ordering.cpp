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

void number_of_way_order(Nodes *nodes, const EdgesTable &edgesTable, const unsigned int startingNode)
{
    std::map<int64_t, unsigned int> numberOfWaysFromNode;
    for(unsigned int i = startingNode; i <nodes->size(); ++i)
    {
        if(edgesTable.find((*nodes)[i].osm_id()) != edgesTable.end())
            numberOfWaysFromNode[(*nodes)[i].osm_id()] = edgesTable.at((*nodes)[i].osm_id()).size();
        else
            numberOfWaysFromNode[(*nodes)[i].osm_id()] = 0;
    }
    std::sort(nodes->begin() + startingNode, nodes->end(),
              [&numberOfWaysFromNode](Node a, Node b)->bool {return numberOfWaysFromNode[a.osm_id()] < numberOfWaysFromNode[b.osm_id()];});
    for(unsigned int i = startingNode; i <nodes->size(); ++i)
    {
        (*nodes)[i].order = i;
    }
}

}
