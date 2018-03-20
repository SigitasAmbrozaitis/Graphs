#ifndef MAPNODE_H
#define MAPNODE_H

#include <QGraphicsPixmapItem>
#include "QBrush"

enum NodeStatus
{
    Available,
    NotAwailable,
    Visited,
    Path,
    Start,
    End
};

//TODO allow node size changing, ideas:
//objective: allow zooming-like behavior, controlled with slider in MainWindow
//1. keep node size as variable, dont allow resizing after map is created
//2. keep node somewhere else, and have method ChangeSize which resizes node;
//Note: this kind of behavior would cost a lot, O(n^2), n = x*y,
//might take a long time especially with slider since that kind of behavior would change
//1.node size(image transform)
//2.node position(need recalculating)
//DO NOT implement if path calculation takes long time already
class MapNode: public QGraphicsPixmapItem
{
public:

    MapNode( int givenX, int givenY,int givenSize = 10, NodeStatus givenStatus = NodeStatus::Available);
    void ChangeStatus(NodeStatus givenStatus);

    NodeStatus status;
    MapNode * previous;
    MapNode * next;

    int x;
    int y;
    int size;


};




#endif // MAPNODE_H
