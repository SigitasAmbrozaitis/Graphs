#include "simplenode.h"
#include "mapnode.h"
SimpleNode::SimpleNode(MapNode *nodeToCopy)
{
    x = nodeToCopy->getX()/nodeToCopy->getSize(); //set x (node size is ignored)
    y = nodeToCopy->getY()/nodeToCopy->getSize(); //set y (node size is ignored)
    nodeStatus = nodeToCopy->getNodeStatus();     //set node status
    index = nodeToCopy->getIndex();               //set node index(index to where the original node is stored in data QList
}
