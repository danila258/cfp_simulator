#ifndef DEFAULTOBJECTS_H
#define DEFAULTOBJECTS_H

#include "UniversalString.h"

#include <vector>
#include <unordered_map>
#include <algorithm>


struct defaultField
{
    UniversalString name;
    UniversalString val;
    UniversalString type;
};

struct defaultParam
{
    UniversalString name;
    std::vector<defaultField> fields;
};

namespace gui
{
    const static std::vector<defaultParam> defaultObjects =
    {
        {"TRTMutex", {}},
        {"TRTBinSemaphore", {{"flState", "1", "bool"}}},
        {"TRTQue", {{"lQSize", "5", "int"}, {"lMSize", "5", "int"}}},
        {"TRTSysTimer", {}},
        {"TRTEvent", {{"flState", "0", "bool"}, {"manualReset", "0", "bool"}}},
        {"TRTCondVar", {}},
        {"TRTSharedMemory", {{"cName", "TRTSharedMemory", "string"}, {"lSize", "5", "int"}, {"map", "1", "bool"}}},
    };

    const static std::unordered_map<std::string, std::vector<defaultParam>> actionMap =
    {
        {"TRTMutex", { {"Lock", {}}, {"Unlock", {}}}},
        {"TRTQue", { {"Write", {{"len", "5", "int"}}} }},
    };
}


#endif //DEFAULTOBJECTS_H
