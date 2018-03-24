#ifndef PATHFINDING_H
#define PATHFINDING_H
#include "simplenode.h"
#include "QObject"

class PathFinder : public QObject
{
    Q_OBJECT
public:
    PathFinder();
    void setup(class QThread & thread);             //connects pathFinding object with thread
    void giveData(class QList<SimpleNode *> & data, int x, int y);//sets mapData, it should be simple data QList from mainWindow


public slots:
    void process();             //main function to launch algorythm
signals:
    void pathSearchStarted();   //
    void pathFound();
    void finished();            //signal that thread has done its work
    void error(class QString err);    //signal to emit erro

private:      
    bool dataIsSet;             //boolean, true is mapData is set
    bool pathIsFound;           //boolean, true if path is found
    QList<SimpleNode *> mapData;//it can be change elswhere so mutexes are needed
    int mapX;                   //map x width
    int mapY;                   //map y height
    SimpleNode * start;         //node from where algorith will start
    SimpleNode * end;           //end of path
    class QMutex * mutex;

    bool setStart();        //sets starting node
    bool findPath();        //runs bfs algorithm
    void traceBack();       //traces back the path
    void findAdjacent(SimpleNode * node, QQueue<SimpleNode *> & dataToAppendTo); //finds neighbor nodes and add them to queue
    void setNeighborNode(SimpleNode * node, SimpleNode * previous);              //sets neighbor node, status, previous
};

#endif // PATHFINDING_H
