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
    pathFinderDone = false;
    start = nullptr;
    end = nullptr;
    mutex = new QMutex();
}

void PathFinder::setup(QThread &thread)
{
    connect(&thread,SIGNAL(started()), this, SLOT(process()));
    connect(this, SIGNAL(finished()), &thread, SLOT(terminate()));
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

bool PathFinder::findPath()
{
    emit pathSearchStarted();
    QQueue<SimpleNode *> queue;
    start->nodeStatus = NodeStatus::visited;
    queue.enqueue(start);

    int counter = 0;
    while(!queue.empty() && !pathIsFound)
    {
        findAdjacent(queue.dequeue(), queue);
        if(counter++ %25 == 0){_sleep(1);}

    }
    pathFinderDone = true;
    return pathIsFound;
}

void PathFinder::traceBack()
{
    if(!pathIsFound) { return; }
    emit pathFound();
    SimpleNode * currentNode = end;
    while(currentNode != start)
    {
        currentNode->nodeStatus = NodeStatus::path;
        mutex->lock();
        PathToPaint.enqueue(currentNode); //add mutex
        mutex->unlock();
        emit paintPathNode();
        _sleep(5);        currentNode = currentNode->previous;
    }
}

void PathFinder::process()
{
    if(!dataIsSet){ qDebug()<< "map data was not set" ;emit finished(); return;}
    if(!setStart()){qDebug()<< "map start was not found" ;emit finished(); return;}
    if(!findPath()){qDebug()<< "map path was not found" ;emit finished(); return;}
    traceBack();
    emit finished();
}

void PathFinder::findAdjacent(SimpleNode *node, QQueue<SimpleNode *> &dataToAppendTo)
{
    int index = node->index;
    SimpleNode * temp;
    //node right neighboor exists
    if(((index + 1) % mapX) != 0  &&  index != mapData.length()-1)
    {
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
        temp = mapData.at(index-1);
        if(temp->nodeStatus != NodeStatus::visited && temp->nodeStatus!= NodeStatus::closed)
        {
            setNeighborNode(temp, node);
            dataToAppendTo.enqueue(temp);
        }
    }
    //node up neighboor
    if(index-mapX >=0)
    {
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
    emit paintActionNode();
}
