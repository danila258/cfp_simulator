#ifndef DEFAULTOBJECTS_H
#define DEFAULTOBJECTS_H

#include "UniversalString.h"

#include <vector>


struct defaultField
{
    UniversalString name;
    UniversalString val;
    UniversalString type;
};

struct defaultObject
{
    UniversalString className;
    std::vector<defaultField> fields;
};

namespace gui
{
    const static std::vector<defaultObject> defaultObjects =
    {
        {"TRTMutex", {}},
        {"TRTBinSemaphore", {{"flState", "1", "bool"}}},
        {"TRTQue", {{"lQSize", "5", "int"}, {"lMSize", "5", "int"}}},
        {"TRTSysTimer", {}},
        {"TRTEvent", {{"flState", "0", "bool"}, {"manualReset", "0", "bool"}}},
        {"TRTCondVar", {}},
        {"TRTSharedMemory", {{"cName", "", "string"}, {"lSize", "5", "int"},
                                         {"map", "1", "bool"}}},
    };
}


#endif //DEFAULTOBJECTS_H
