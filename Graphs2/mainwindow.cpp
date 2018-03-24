#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "globals.h"
#include "mapnode.h"
#include "QDebug"
#include "windows.h"
#include "simplenode.h"
#include "QThread"
#include "QTimer"
#include "QMutex"


//IDEA nr1
//buttons only set booleans that allow:
// to work methods that are launched with timers
// for example:
// if button pressed set generatingallowed = true
// in the meantime there is launched method generate connected to timer and return if !generationallowed
//IDEA nr2
//generation generates nodes that are added to the scene later
//after mapnodes are generated, simple nodes are created from them(location, status)
//Graphs class takes in simple node array, launches bfs in separate thread: return queue of nodes for to display actaions taken and queue of nodes to display path
//on main thread there is launched 2 timers, one deques displayed actions queue and edits map nodes, other does the same for path queue
//both timers start working after booleans are set via interface(buttons)

//both ideas should not lock interface and display realtime pathfinding

NodeStatus status = NodeStatus::nothing;    //status that says what type is selected with toggles for painting
QQueue<SimpleNode *> NodesToPaint;
QQueue<SimpleNode *> PathToPaint;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //inicialization
    status = NodeStatus::nothing;
    findPressed = false;
    mapGenerated = false;
    mapSize = 5;
    mapNodeSize = 2;
    nodeRarity = 0;

    //dynamic allocations
    mutex = new QMutex();
    generator =  new Generator();
    pathFinding = new PathFinder();
    pathFindingThread = new QThread();

    //set up transform
    defaultViewTransform = ui->graphicsView->transform();

    //Set up scene
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui->graphicsView->setUpdatesEnabled(true);

    //set up zoom cganging
    connect(ui->ZoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setViewScale()));

    //set up map size changing
    connect(ui->MapSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(setMapSize()));
    connect(ui->MapSizeSlider, SIGNAL(valueChanged(int)), ui->MapSizeSpinBox, SLOT(setValue(int)));
    connect(ui->MapSizeSpinBox, SIGNAL(valueChanged(int)), ui->MapSizeSlider, SLOT(setValue(int)));

    //set up map node size changing
    connect(ui->MapNodeSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(setMapNodeSize()));
    connect(ui->MapNodeSizeSlider, SIGNAL(valueChanged(int)), ui->MapNodeSizeSpinBox, SLOT(setValue(int)));
    connect(ui->MapNodeSizeSpinBox, SIGNAL(valueChanged(int)), ui->MapNodeSizeSlider, SLOT(setValue(int)));

    //set up awailable node rarity
    connect(ui->RaritySlider, SIGNAL(valueChanged(int)), this, SLOT(setNodeRarity()));
    connect(ui->RaritySlider, SIGNAL(valueChanged(int)), ui->RaritySpinBox, SLOT(setValue(int)));
    connect(ui->RaritySpinBox, SIGNAL(valueChanged(int)), ui->RaritySlider, SLOT(setValue(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createSimpleNodeContainer()
{
    SimpleNode * newNode;
    for(MapNode * node : data)
    {
        newNode = new SimpleNode(node);
        simpleData.append(newNode);
    }
}

void MainWindow::deleteOldMap()
{
    //clearing scene
    scene->clear();

    //clearing all containers
    data.clear();
    NodesToPaint.clear();
    PathToPaint.clear();
    simpleData.clear();

    //deleting used objects
    delete generator;
    delete pathFinding;
    delete pathFindingThread;
    findPressed = false;
}

void MainWindow::setViewScale()
{
    float currentScale = ui->ZoomSlider->value();
    if(currentScale<0)
    {
        currentScale = -1 / currentScale;
    }
    if(currentScale ==0)
    {
        ui->graphicsView->setTransform(defaultViewTransform);
    }else
    {
        QTransform transform = defaultViewTransform;

        transform.scale(currentScale, currentScale);
        ui->graphicsView->setTransform(transform);
    }
}

void MainWindow::setMapSize()
{
    mapSize = ui->MapSizeSlider->value();
}

void MainWindow::setMapNodeSize()
{
    mapNodeSize = ui->MapNodeSizeSlider->value();
}

void MainWindow::setNodeRarity()
{
    nodeRarity = ui->RaritySlider->value();
}

void MainWindow::paintNode()
{
    if(NodesToPaint.empty()){return;}
    mutex->lock();
    SimpleNode * node = NodesToPaint.dequeue();
    mutex->unlock();
    MapNode * nodeToEdit = data.at(node->index);
    nodeToEdit->setNodeStatus(node->nodeStatus);
}

void MainWindow::paintPathNode()
{
    if(PathToPaint.empty()){return;}
    mutex->lock();
    SimpleNode * node = PathToPaint.dequeue();
    mutex->unlock();
    MapNode * nodeToEdit = data.at(node->index);
    nodeToEdit->setNodeStatus(node->nodeStatus);
}

void MainWindow::on_StartEnum_toggled(bool checked)
{
    if(checked)
    {
        status = NodeStatus::start;
    }
}

void MainWindow::on_EndEnum_toggled(bool checked)
{
    if(checked)
    {
        status = NodeStatus::end;
    }
}

void MainWindow::on_AwailableEnum_toggled(bool checked)
{
    if(checked)
    {
        status = NodeStatus::awailable;
    }
}

void MainWindow::on_ClosedEnum_toggled(bool checked)
{
    if(checked)
    {
        status = NodeStatus::closed;
    }
}

void MainWindow::on_FindButton_clicked()
{
    if(!mapGenerated || findPressed) { return; }
    pathFinding->setup(*pathFindingThread);

    connect(pathFinding, SIGNAL(paintActionNode()), this, SLOT(paintNode()));
    connect(pathFinding, SIGNAL(paintPathNode()), this, SLOT(paintPathNode()));

    createSimpleNodeContainer();
    pathFinding->giveData(simpleData,mapSize,mapSize);
    pathFinding->moveToThread(pathFindingThread);
    pathFindingThread->start();
    findPressed = true;
}

void MainWindow::on_GenerateMapButton_clicked()
{
    deleteOldMap();
    qDebug() << "deleted";
    pathFinding = new PathFinder();
    pathFindingThread = new QThread();
    generator = new Generator();

    generator->setOptions(mapSize, mapSize, nodeRarity, mapNodeSize);
    generator->generate();
    data = generator->getGenerated();

    int index = 0;
    for(MapNode * node : data)
    {
        node->setIndex(index++);
        scene->addItem(node);
    }
    mapGenerated = true;
}
