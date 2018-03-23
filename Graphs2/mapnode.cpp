#include "mapnode.h"
#include "QDebug"
#include "QPainter"
#include "globals.h"
#include "QGraphicsSceneMouseEvent"

MapNode::MapNode(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent):QGraphicsRectItem(x, y, width, height, parent)
{
    pressed =  true;
    painted = false;
    changed = false;
    setNodeStatus(NodeStatus::awailable);
    setLocation(x, y);
    size = width;

}

void MapNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rectangle = boundingRect();
    QBrush brush(Qt::blue);
    //initial paint so that the view would have one color
    if(!painted)
    {
        painter->fillRect(rectangle, brush);
        painter->drawRect(rectangle);
        painted = true;
    }
    //sets brush color depending on node status
    if(pressed || changed)
    {
        switch(nodeStatus)
        {
            case NodeStatus::start:
            brush.setColor(Qt::red);
            break;

            case NodeStatus::end:
            brush.setColor(Qt::green);
            break;

            case NodeStatus::awailable:
            brush.setColor(Qt::blue);
            break;

            case NodeStatus::visited:
            brush.setColor(Qt::yellow);
            break;

            case NodeStatus::path:
            brush.setColor(Qt::red);
            break;

            case NodeStatus::closed:
            brush.setColor(Qt::black);
            break;

            case NodeStatus::nothing:
            brush.setColor(Qt::blue);
            break;
        }
        changed = false;
    }
    //paints node
    painter->fillRect(rectangle, brush);
    painter->drawRect(rectangle);
}

void MapNode::setLocation(int x, int y)
{
    mapX = x;
    mapY = y;
}

void MapNode::setSize(int size)
{
    this->size = size;
}

void MapNode::setNodeStatus(NodeStatus statusToSet)
{
    nodeStatus = statusToSet;
    changed = true;
    update(boundingRect());
}

void MapNode::setIndex(int index)
{
    nodeIndex = index;
}

int MapNode::getX()
{
    return mapX;
}

int MapNode::getY()
{
    return mapY;
}

int MapNode::getSize()
{
    return size;
}

int MapNode::getIndex()
{
    return nodeIndex;
}

NodeStatus MapNode::getNodeStatus()
{
    return nodeStatus;
}

void MapNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //if pressed but status changed
    if(status != nodeStatus && pressed)
    {
        pressed = false;
    }
    //if not pressed yet
    if(!pressed)
    {
        pressed = true;
        setNodeStatus(status);
    }
}

void MapNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //update();
}

void MapNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

}
