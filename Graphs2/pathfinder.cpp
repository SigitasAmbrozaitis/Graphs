#include "pathfinder.h"
#include "QDebug"
#include "QThread"
#include "QQueue"
#include "globals.h"
#include "QMutex"
PathFinder::PathFinder()
{
    dataIsSet = false;
    pathIsFound = false;
    start = nullptr;
    end = nullptr;
    mutex = new QMutex();
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
        if(node->nodeStatus == NodeStatus::start)
        {
            valueToReturn = true;
            start = node;
            break;
        }
    }
    return valueToReturn;
}
//TODO find crash couse in find path or deeper
bool PathFinder::findPath()
{
    emit pathSearchStarted();
    qDebug() << "searching for path";
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
    qDebug() << "tracing back";
    SimpleNode * currentNode = end;
    while(currentNode != nullptr)
    {
        currentNode->nodeStatus = NodeStatus::path;
        mutex->lock();
        PathToPaint.enqueue(currentNode); //add mutex
        mutex->unlock();
        currentNode = currentNode->previous;
    }
    qDebug() << "finished tracing back";
}

void PathFinder::process()
{
    qDebug() << "thread started";
    qDebug() << "setting up data";
    if(!dataIsSet){ qDebug()<< "map data was not set" ; return;}
    qDebug() << "setting up start";
    if(!setStart()){qDebug()<< "map start was not found" ; return;}
    qDebug() << "setting up path";
    if(!findPath()){qDebug()<< "map path was not found" ; return;}
    qDebug() << "setting up traceback";
    emit pathFound();
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
        qDebug() << "right";
        temp = mapData.at(index+1);
        if(temp->nodeStatus != NodeStatus::visited && temp->nodeStatus!= NodeStatus::closed)
        {
            setNeighborNode(temp, node);
            dataToAppendTo.enqueue(temp);
        }

    }
    //node left neighboor
    if(((index)%mapX) != 0 && index != 0)
    {
        qDebug() << "left";
        qDebug() << "index:" << index;
        temp = mapData.at(index-1);
        qDebug() << "index:" <<temp->index << " status:" << temp->nodeStatus << " x:" << temp->x << " y:" << temp->y;
        if(temp->nodeStatus != NodeStatus::visited && temp->nodeStatus!= NodeStatus::closed)
        {
            setNeighborNode(temp, node);
            dataToAppendTo.enqueue(temp);
        }
    }
    //node up neighboor
    if(index-mapX >=0)
    {
        qDebug() << "up";
        temp = mapData.at(index-mapX);
        if(temp->nodeStatus != NodeStatus::visited && temp->nodeStatus!= NodeStatus::closed)
        {
            setNeighborNode(temp, node);
            dataToAppendTo.enqueue(temp);
        }
    }
    //node down neighboor
    if(index < mapX * (mapY-1))
    {
        qDebug() << "down";
        temp = mapData.at(index+mapX);
        if(temp->nodeStatus != NodeStatus::visited && temp->nodeStatus!= NodeStatus::closed)
        {
            setNeighborNode(temp, node);
            dataToAppendTo.enqueue(temp);
        }
    }

}

void PathFinder::setNeighborNode(SimpleNode *node, SimpleNode *previous)
{
    node->previous = previous;
    if(node->nodeStatus == NodeStatus::end)
    {
        end = node;
        pathIsFound = true;
    }
    node->nodeStatus = NodeStatus::visited;

    mutex->lock();
    NodesToPaint.enqueue(node); //add mutex
    mutex->unlock();
}
