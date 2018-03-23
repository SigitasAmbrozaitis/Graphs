#ifndef GENERATOR_H
#define GENERATOR_H
#include "QList"

class Generator
{
public:
    Generator();                                            //empty constructor
    Generator(int x, int y, int rate, int nodeSize);        //constructor that sets generated map parameters
    void setOptions(int x, int y, int rate, int nodeSize);  //sets generated map parameters
    QList<class MapNode *> getGenerated();                  //returns generated map in QList
    void generate();                                        //generates map and stores in privare QList data
private:
    QList<class MapNode *> data;    //Qlist containinng generated map
    bool ready;                     //boolean true if node parameters are set, does not allow generation if false
    int x, y;                       //map size
    int nodeSize;                   //node size in the map
    int rate;                       //awailable node spawn rate
};

#endif // GENERATOR_H
