#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void Dummy();
    void LaunchMapGeneration();
private slots:
    void on_Start_clicked();

private:
    Ui::MainWindow *ui;

    QGraphicsScene * scene;

    class MapGenerator * mapGenerator;
    QList<class MapNode *> map;
    
    int windowSize;
    int nodeSize;

    bool generationLocked;
public slots:
    void SetNodeValueWithSlider();
    void SetWindowValueWithSlider();
};

#endif // MAINWINDOW_H
