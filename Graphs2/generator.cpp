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
    qDebug() << "options set";
}

QList<MapNode *> Generator::getGenerated()
{
    qDebug() << "returning generated";
    return data;
}

void Generator::generate()
{
    if(!ready){return;}
    MapNode * node;
    int rnd;
    for(int j=0; j<x; ++j)
    {
        for(int i=0; i<y; ++i)
        {
            node = new MapNode(j*nodeSize,i*nodeSize,nodeSize, nodeSize);
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
