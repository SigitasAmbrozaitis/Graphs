#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "globals.h"
#include "mapnode.h"
#include "QDebug"
#include "windows.h"
#include "simplenode.h"
#include "QThread"
#include "QTimer"


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
    status = NodeStatus::nothing;
    //dynamic allocations
    generator =  new Generator();
    pathFinding = new PathFinder();
    pathFindingThread = new QThread();
    paintTimer = new QTimer();
    pathTimer = new QTimer();

    defaultViewTransform = ui->graphicsView->transform();
    pathFinding->setup(*pathFindingThread);

    QObject::connect(ui->ZoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setViewScale()));

    connect(pathFinding, SIGNAL(pathSearchStarted()), this, SLOT(startPainting()));
    connect(pathFinding,SIGNAL(pathFound()), this, SLOT(showPath()));

    connect(paintTimer, SIGNAL(timeout()), this, SLOT(paintNode()));
    connect(pathTimer, SIGNAL(timeout()), this, SLOT(paintPathNode()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::test()
{

    QGraphicsScene * scene = new QGraphicsScene(this);
    //scene->setSceneRect(0, 0, 1000, 1000);
    ui->graphicsView->setScene(scene);
    int size =10;
    for(int j=0; j<30;++j)
    {
        for(int i = 0; i<30; ++i)
        {
            MapNode * node = new MapNode(i*size, j*size, size, size);
            scene->addItem(node);
            data.append(node);
        }
    }



}

void MainWindow::testGenerator()
{
    QGraphicsScene * scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui->graphicsView->setUpdatesEnabled(true);

    generator->setOptions(50,40,100, 20);

    generator->generate();
    data = generator->getGenerated();
    qDebug() << "started iterating";
    int index = 0;
    for(MapNode * node : data)
    {
        node->setIndex(index++);
        scene->addItem(node);
    }
    pathTimer->stop();
    paintTimer->stop();

    qDebug() << "creating simple map";
    //createSimpleNodeContainer();
    qDebug() << "simple map created";

   /* pathFinding->giveData(simpleData,50,50);
    pathFinding->moveToThread(pathFindingThread);
    pathFindingThread->start();*/

}

void MainWindow::createSimpleNodeContainer()
{
    SimpleNode * newNode;
    for(MapNode * node : data)
    {
        newNode = new SimpleNode(node);
        simpleData.append(newNode);
        qDebug() << "index:" << newNode->index << " status:" << newNode->nodeStatus << " x:" << newNode->x << " y:" << newNode->y;
    }
}

void MainWindow::setViewScale()
{
    float currentScale = ui->ZoomSlider->value();
    if(currentScale<0)
    {
        currentScale = -1 / currentScale;
    }
    QTransform transform = defaultViewTransform;

    transform.scale(currentScale, currentScale);
    ui->graphicsView->setTransform(transform);

}

void MainWindow::startPainting()
{
    qDebug() << "timer started";
    paintTimer->start();
}

void MainWindow::showPath()
{
    pathTimer->start();
}

void MainWindow::paintNode()
{
    if(NodesToPaint.empty()){return;}
    SimpleNode * node = NodesToPaint.dequeue();
    MapNode * nodeToEdit = data.at(node->index);
    nodeToEdit->setNodeStatus(node->nodeStatus);
}

void MainWindow::paintPathNode()
{
    if(PathToPaint.empty()){return;}
    SimpleNode * node = PathToPaint.dequeue();
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

void MainWindow::on_pushButton_clicked()
{
    testGenerator();
    qDebug() << "exited generator";
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
    createSimpleNodeContainer();
    pathFinding->giveData(simpleData,50,40);
    pathFinding->moveToThread(pathFindingThread);
    pathFindingThread->start();
}
