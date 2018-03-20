#include "pathfinder.h"
#include "QDebug"
#include "QThread"

PathFinder::PathFinder()
{
    dataIsSet = false;
    pathIsFound = false;
}

void PathFinder::setup(QThread &thread)
{
    connect(&thread,SIGNAL(started()), this, SLOT(process()));
}

void PathFinder::giveData(QList<SimpleNode *> &data)
{
    mapData = data;
    dataIsSet = true;
}

void PathFinder::process()
{
    if(!dataIsSet){return;}
    //do all process of algorytm here
}
