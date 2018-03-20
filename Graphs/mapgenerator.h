#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include <QList>
#include "mapnode.h"
#include <QGraphicsScene>
#include <QThread>

class MapGenerator : public QThread
{
public:
    MapGenerator(QList<MapNode*> & givenMap, QGraphicsScene * givenScene, int givenMapSize = 50, int givenNodeSize = 10, int givenRate = 50);
    QList<MapNode*> GetMap();
    void run();
private:
    QList<MapNode *>  map;
    QGraphicsScene * scene;
    int mapSize;
    int nodeSize;
    int rate;

    void GenerateMap();
    void DeleteOldMap();

};

#endif // MAPGENERATOR_H
