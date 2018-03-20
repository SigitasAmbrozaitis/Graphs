#ifndef SIMPLENODE_H
#define SIMPLENODE_H
#include "nodestatus.h"

class SimpleNode
{
public:
    SimpleNode(class MapNode * nodeToCopy); //constructor copies node that is shown in graphic view

    int x, y;               //node location
    int index;              //node index in array
    NodeStatus nodeStatus;  //node status
    SimpleNode * previous;  //pointer to where we came to this node from (is set during the search of node neighboors)
    SimpleNode * next;      //pointer to where this node goes to (is set during trace back)
};

#endif // SIMPLENODE_H
