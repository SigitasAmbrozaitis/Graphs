#ifndef GLOBALS_H
#define GLOBALS_H
#include "nodestatus.h"
#include "simplenode.h"
#include "QQueue"

extern NodeStatus status;                   //stores what nodeStatus is currently picked in GUI
extern QQueue<SimpleNode *> NodesToPaint;   //stores nodes that need to be painted as visited
extern QQueue<SimpleNode *> PathToPaint;    //stores nodes that need to be painted as path
//extern QList<SimpleNode *> simpleData;

#endif // GLOBALS_H
