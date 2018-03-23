#include "pathfinder.h"
#include "QDebug"
#include "QThread"
#include "QQueue"
#include "globals.h"
PathFinder::PathFinder()
{
    dataIsSet = false;
    pathIsFound = false;
    start = nullptr;
    end = nullptr;
}

void PathFinder::setup(QThread &thread)
{
    connect(&thread,SIGNAL(started()), this, SLOT(process()));
}

void PathFinder::giveData(QList<SimpleNode *> &data, int x, int y)
{
    mapX = x;
    mapY = y;
    mapData = data;
    dataIsSet = true;
}

bool PathFinder::setStart()
{
    bool valueToReturn = false;
    for(SimpleNode* node: mapData)
    {
        qDebug() << node->nodeStatus;
        if(node->nodeStatus == NodeStatus::start)
        {
            valueToReturn = true;
            start = node;
            break;
        }
    }
    return valueToReturn;
}

bool PathFinder::findPath()
{
    QQueue<SimpleNode *> queue;
    start->nodeStatus = NodeStatus::visited;
    queue.enqueue(start);

    while(!queue.empty() && !pathIsFound)
    {
        findAdjacent(queue.dequeue(), queue);
    }
    return pathIsFound;
}

void PathFinder::traceBack()
{
    SimpleNode * currentNode = end;
    while(currentNode != nullptr)
    {
        currentNode->nodeStatus = NodeStatus::path;
        PathToPaint.enqueue(currentNode); //add mutex
        currentNode = currentNode->previous;
    }
}

void PathFinder::process()
{
    qDebug() << "thread started";
    if(!dataIsSet){ qDebug()<< "map data was not set" ; return;}
    if(!setStart()){qDebug()<< "map start was not found" ; return;}
    if(!findPath()){qDebug()<< "map path was not found" ; return;}
    traceBack();
    //emit signal that work is done
    //do all process of algorytm here
    qDebug() << "thread ended";
}

void PathFinder::findAdjacent(SimpleNode *node, QQueue<SimpleNode *> &dataToAppendTo)
{
    int index = node->index;
    SimpleNode * temp;
    //node right neighboor exists
    if(((index + 1) % mapX) != 0  &&  index != mapData.length()-1)
    {
        temp = mapData.at(index+1);
        setNeighborNode(temp, node);
        dataToAppendTo.enqueue(temp);
    }
    //node left neighboor
    if(index%mapX == 0 && index != 0)
    {
        temp = mapData.at(index-1);
        setNeighborNode(temp, node);
        dataToAppendTo.enqueue(temp);
    }
    //node up neighboor
    if(index >= mapX)
    {
        temp = mapData.at(index-mapX);
        setNeighborNode(temp, node);
        dataToAppendTo.enqueue(temp);
    }
    //node down neighboor
    if(index <= mapX * mapY)
    {
        temp = mapData.at(index+mapX);
        setNeighborNode(temp, node);
        dataToAppendTo.enqueue(temp);
    }

}

void PathFinder::setNeighborNode(SimpleNode *node, SimpleNode *previous)
{
    qDebug() << " hello";
    node->previous = previous;
    if(node->nodeStatus == NodeStatus::end)
    {
        end = node;
        pathIsFound = true;
    }
    node->nodeStatus = NodeStatus::visited;
    NodesToPaint.enqueue(node); //add mutex

}
