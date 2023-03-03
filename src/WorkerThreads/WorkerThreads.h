#ifndef WORKERTHREADS_H
#define WORKERTHREADS_H

#include "ConfigParser.h"

#include <queue>
#include <thread>
#include <functional>


void threadFunction(const std::vector<dataStructure>& threadConfig);
void workerThreads(const std::vector<threadConfig>& config);

#endif // WORKERTHREADS_H
