#ifndef GLOBALS_H
#define GLOBALS_H
#include "nodestatus.h"
#include "simplenode.h"
#include "QQueue"
//stores what nodeStatus is currently picked in GUI
extern NodeStatus status;
extern QQueue<SimpleNode *> NodesToPaint;
extern QQueue<SimpleNode *> PathToPaint;
//extern QList<SimpleNode *> simpleData;

#endif // GLOBALS_H
