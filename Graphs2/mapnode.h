#ifndef MAPNODE_H
#define MAPNODE_H

#include "QGraphicsRectItem"
#include "nodestatus.h"

class MapNode : public QGraphicsRectItem
{
public:
    MapNode(qreal x, qreal y, qreal width, qreal height, QGraphicsItem * parent = 0);

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;
    void setLocation(int x, int y);             //sets node location
    void setSize(int size);                     //sets node size
    void setNodeStatus(NodeStatus statusToSet); //sets node status
    void setIndex(int index);                   //sets index where the node is stored in array

    int getX();     //returns mapX
    int getY();     //returns mapY
    int getSize();  //returns nodeSize
    int getIndex(); //returns nodeIndex
    NodeStatus getNodeStatus(); //return nodeStatus

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;    //mouse press event
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event)override;   //mouse release event TODO implement or delete
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)override;       //mouse move event TODO implement or delete
private:

    int mapX, mapY;
    int size;
    int nodeIndex;
    NodeStatus nodeStatus;

    bool pressed;   //boolean true if node is pressed
    bool painted;   //boolean true if node is painted  TODO inspect if used as intended
    bool changed;   //boolean true if node was changed TODO inspect if used as intended

};

#endif // MAPNODE_H
