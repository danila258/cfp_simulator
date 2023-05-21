#ifndef DATATRANSFERINTERFACE_H
#define DATATRANSFERINTERFACE_H

#include "UniversalString.h"

#include <vector>


struct objectContent
{
    UniversalString className;
    UniversalString varName;
    size_t startId;
    size_t count;
    std::vector<UniversalString> args;
};

struct threadContent
{
    size_t number;
    std::vector<objectContent> objects;
};

struct actionContent
{
    size_t thread;
    UniversalString action;
    UniversalString className;
    size_t id;
    double pause = 0;
    UniversalString varName;
    UniversalString funcName;
    std::vector<UniversalString> args;
};


#endif //DATATRANSFERINTERFACE_H
