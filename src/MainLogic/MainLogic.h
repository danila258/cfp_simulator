#ifndef MAINLOGIC_H
#define MAINLOGIC_H

#include "ConfigParser.h"
#include "WorkerThreads.h"

#include <iostream>


class MainLogic
{
public:
    void startProgram(int argc, char* argv[]);
    void runThreads(const std::string& path);
};

#endif //MAINLOGIC_H
