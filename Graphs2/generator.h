#ifndef GENERATOR_H
#define GENERATOR_H
#include "QList"

class Generator
{
public:
    Generator();
    Generator(int x, int y, int rate, int nodeSize);
    void setOptions(int x, int y, int rate, int nodeSize);
    QList<class MapNode *> getGenerated();
    void generate();
private:
    QList<class MapNode *> data;
    bool ready;
    int x, y;
    int nodeSize;
    int rate;
};

#endif // GENERATOR_H
