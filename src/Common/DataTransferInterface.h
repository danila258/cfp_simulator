#ifndef DATATRANSFERINTERFACE_H
#define DATATRANSFERINTERFACE_H

#include "UniversalString.h"

#include <vector>


struct objectContent
{
    UniversalString className;
    UniversalString varName;
    int count;
    std::vector<UniversalString> args;
};

struct threadContent
{
    int number;
    std::vector<objectContent> objects;
};

struct actionContent
{
    UniversalString action;
    UniversalString className;
    UniversalString varName;
    UniversalString funcName;
    std::vector<UniversalString> args;
};


#endif //DATATRANSFERINTERFACE_H
