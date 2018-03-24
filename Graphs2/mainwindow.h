#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QList"


#include "generator.h"
#include "pathfinder.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void test();            //test function TODO delete
    void testGenerator();   //test function TODO delete

private:
    Ui::MainWindow *ui;

    QList<class MapNode *> data;            //container to store map, added in scene
    QList<class SimpleNode *> simpleData;   //container to store simple map
    QTransform  defaultViewTransform;       //contains default view for scaling
    class QGraphicsScene * scene;
    class Generator * generator;            //object that generates data map
    class PathFinder * pathFinding;         //object that find path in simpleData
    class QThread * pathFindingThread;      //thread that is responsible for BFS work
    class QMutex * mutex;                   //mutex that controls NodesToPaint PathToPaint access


    int mapSize;
    int mapNodeSize;
    int nodeRarity;

    bool findPressed; //if true path search already working or path is found
    bool mapGenerated;//if true map is generated

    void createSimpleNodeContainer();       //creates QList contianing SimpleNodes from MapNodes
    void deleteOldMap();

public slots:
    void setViewScale();        //changes view scale, connected with ZoomSlider
    void setMapSize();          //changes mapSize connected to MapSizeSlider
    void setMapNodeSize();      //changes mapNodeSize connected to MapNodeSizeSlider
    void setNodeRarity();       //changes nodeRarity connected to nodeRaritySLider

    void paintNode();
    void paintPathNode();

private slots:
    void on_StartEnum_toggled(bool checked);    //toggle in GUI if picked changes global variable status
    void on_EndEnum_toggled(bool checked);      //toggle in GUI if picked changes global variable status
    void on_AwailableEnum_toggled(bool checked);//toggle in GUI if picked changes global variable status
    void on_ClosedEnum_toggled(bool checked);   //toggle in GUI if picked changes global variable status
    void on_FindButton_clicked();
    void on_GenerateMapButton_clicked();

signals:
    void pathPainted();
};


#endif // MAINWINDOW_H
