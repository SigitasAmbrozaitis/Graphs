#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mapnode.h"
#include "mapgenerator.h"

#include <QDebug>
#include <QFuture>
#include <QtConcurrent>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 25000, 25000);//TODO allow size change connected to node size
    ui->GraphView->setScene(scene);

    generationLocked = false;


    QObject::connect(ui->NodeSlider, SIGNAL(valueChanged(int)), this, SLOT(SetNodeValueWithSlider())); 
    QObject::connect(ui->MapSlider, SIGNAL(valueChanged(int)), this, SLOT(SetWindowValueWithSlider()));
    //mapGenerator = new MapGenerator(scene);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Dummy()//TODO delete
{
    qDebug() << "Thread working";


}

void MainWindow::LaunchMapGeneration()
{
    map = mapGenerator->GetMap();
}

void MainWindow::on_Start_clicked()
{
    if(generationLocked){return;}
    generationLocked = true;

    scene->setSceneRect(0, 0,windowSize*nodeSize,windowSize*nodeSize);
    mapGenerator = new MapGenerator(map, scene, windowSize,nodeSize);
    mapGenerator->start();

    //TODO implement
    //High priority: Create new one time thread for map generation
    //Low priority: Create progress bar [0:mapSize^2]
    generationLocked = false;
}

void MainWindow::SetNodeValueWithSlider()
{
   nodeSize = ui->NodeSlider->value();
   qDebug() << "setting value to:" << nodeSize;
}

void MainWindow::SetWindowValueWithSlider()
{
    windowSize = ui->MapSlider->value();
    qDebug() << "setting value to:" << windowSize;
}
