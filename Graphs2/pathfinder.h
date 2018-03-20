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
    void giveData(class QList<SimpleNode *> & data);//sets mapData, it should be simple data QList from mainWindow

public slots:
    void process();             //main function to launch algorythm
signals:
    void finished();            //signal that thread has done its work
    void error(QString err);    //signal to emit erro

private:
    bool dataIsSet;             //boolean, true is mapData is set
    bool pathIsFound;           //boolean, true if path is found
    QList<SimpleNode *> mapData;    //it can be change elswhere so mutexes are needed
};

#endif // PATHFINDING_H
