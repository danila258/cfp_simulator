#ifndef WORKERTHREADS_H
#define WORKERTHREADS_H

#include "CustomMutex.h"
#include "CustomQueue.h"
#include "ConfigParser.h"

#include "log/TRTLog.h"
#include "log/TRTLogSender.h"

#include <deque>
#include <thread>
#include <memory>


void threadFunction(const threadConfig& threadConfig);
void workerThreads(const std::vector<threadConfig>& config);


#endif // WORKERTHREADS_H
