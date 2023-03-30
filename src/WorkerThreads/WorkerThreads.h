#ifndef WORKERTHREADS_H
#define WORKERTHREADS_H

#include "CustomMutex.h"
#include "CustomQueue.h"
#include "ConfigParser.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/logger.h"

#include <deque>
#include <thread>
#include <memory>


void threadFunction(const threadConfig& threadConfig, const std::shared_ptr<spdlog::logger>& logger);
void workerThreads(const std::vector<threadConfig>& config, const std::shared_ptr<spdlog::logger>& logger);


#endif // WORKERTHREADS_H
