#include "generator.h"
#include "mapnode.h"
#include "QDebug"
Generator::Generator()
{
    ready =  false;
}

Generator::Generator(int x, int y, int rate, int nodeSize)
{
    ready = true;
    setOptions(x,y,rate, nodeSize);
}

void Generator::setOptions(int x, int y, int rate, int nodeSize)
{
    this->x = x;
    this->y = y;
    this->rate = rate;
    if(this->rate < 0 && this->rate>100)
    {
        this->rate = 50;
    }
    this->nodeSize = nodeSize;
    ready = true;
}

QList<MapNode *> Generator::getGenerated()
{
    return data;
}

void Generator::generate()
{
    if(!ready){return;}
    MapNode * node;
    int rnd;
    for(int j=0; j<y; ++j)
    {
        for(int i=0; i<x; ++i)
        {
            node = new MapNode(i*nodeSize,j*nodeSize,nodeSize, nodeSize);
            rnd = rand()%100;
            if(rnd < rate)
            {
                node->setNodeStatus(NodeStatus::awailable);
            }else
            {
                node->setNodeStatus(NodeStatus::closed);
            }
            data.append(node);
        }
    }
}
