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

    void test();
    void testGenerator();

private:
    Ui::MainWindow *ui;
    QList<class MapNode *> data;            //container to store map, added in scene
    QList<class SimpleNode *> simpleData;   //container to store simple map
    class Generator * generator;            //object that generates data map
    class PathFinder * pathFinding;        //object that find path in simpleData
    class QThread * pathFindingThread;
    QTransform  defaultViewTransform;       //contains default view for scaling



    void createSimpleNodeContainer();

public slots:
    void setViewScale();                    //changes view scale, connected with ZoomSlider

private slots:
    void on_StartEnum_toggled(bool checked);
    void on_EndEnum_toggled(bool checked);
    void on_AwailableEnum_toggled(bool checked);
    void on_pushButton_clicked();
    void on_ClosedEnum_toggled(bool checked);
};


#endif // MAINWINDOW_H
