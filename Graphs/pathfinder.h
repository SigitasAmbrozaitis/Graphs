#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <QList>
#include "mapnode.h"

class PathFinder
{
public:
    PathFinder(QList<MapNode *> givenMap);

private:
    QList<MapNode *> map;

};

#endif // PATHFINDER_H
