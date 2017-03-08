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

void order_with_num_of_roads(Nodes *nodes)
{
    std::sort(nodes->begin(), nodes->end(), [](Node a, Node b) {return a.numOfWays < b.numOfWays;});
    for(unsigned int i = 0; i <nodes->size(); ++i)
    {
        (*nodes)[i].order = i;
    }
}

}
