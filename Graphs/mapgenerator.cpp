#include "mapgenerator.h"
#include <QDebug>

MapGenerator::MapGenerator(QList<MapNode*> & givenMap,QGraphicsScene *givenScene, int givenMapSize, int givenNodeSize, int givenRate): QThread()
{
    map = givenMap;
    scene = givenScene;
    mapSize = givenMapSize;
    nodeSize = givenNodeSize;
    rate = givenRate;
}

QList<MapNode *> MapGenerator::GetMap()
{
    DeleteOldMap();
    GenerateMap();

    return map;
}

void MapGenerator::run()
{
    DeleteOldMap();
    GenerateMap();
}

void MapGenerator::GenerateMap()
{
    //qsrand(); TODO implement random seed genration after finished testing
    MapNode * creatingNode;
    for(int i = 0; i<mapSize; ++i)
    {
        for(int j=0; j<mapSize; ++j)
        {
            int number = qrand()%100;
            if(number >rate)//not available
            {
                creatingNode = new MapNode(i,j,nodeSize,NodeStatus::NotAwailable);
            }else
            {
                creatingNode = new MapNode(i,j,nodeSize, NodeStatus::Available);
            }
            creatingNode->setPos(i*nodeSize,j*nodeSize);
            scene->addItem(creatingNode);
            map.append(creatingNode);
            scene->update();
        }
    }
}

void MapGenerator::DeleteOldMap()
{
    scene->clear();
    for(MapNode * item:map)
    {
        delete item;
    }
    map.clear();
}
