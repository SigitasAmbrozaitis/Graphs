#include "mapnode.h"
#include <QDebug>
#include <QPainter>
#include <QPixmap>

MapNode::MapNode(int givenX, int givenY, int givenSize, NodeStatus givenStatus) : QGraphicsPixmapItem()
{
    x = givenX;
    y = givenY;
    size = givenSize;
    previous = nullptr;
    next = nullptr;
    ChangeStatus(givenStatus);

}

void MapNode::ChangeStatus(NodeStatus givenStatus)
{
    status = givenStatus;
    switch(status)
    {
        //possible way to achieve painting, this->paint(...);
        case NodeStatus::Available:
            this->setPixmap(QPixmap(":/Colors/Recourses/available.png").scaled(size, size, Qt::KeepAspectRatio));
            break;
        case NodeStatus::NotAwailable: //paint black
            this->setPixmap(QPixmap(":/Colors/Recourses/notAvailable.png").scaled( size,  size, Qt::KeepAspectRatio));
            break;
        case NodeStatus::Visited: // paint yellow
            this->setPixmap(QPixmap(":/Colors/Recourses/current.png").scaled( size,  size, Qt::KeepAspectRatio));
            break;
        case NodeStatus::Path: // paint red
            this->setPixmap(QPixmap(":/Colors/Recourses/current.png").scaled( size,  size, Qt::KeepAspectRatio));
            break;
        case NodeStatus::Start:
            this->setPixmap(QPixmap(":/Colors/Recourses/start.png").scaled( size,  size, Qt::KeepAspectRatio));
            break;
        case NodeStatus::End:
            this->setPixmap(QPixmap(":/Colors/Recourses/end.png").scaled( size,  size, Qt::KeepAspectRatio));
            break;
        default: //do nothing
            break;
    };
}
