#ifndef WORKERTHREADS_H
#define WORKERTHREADS_H

#include "DataTransferInterface.h"
#include "UniversalString.h"
#include "ObjectFactory.h"
#include "Objects.h"

#include "log/TRTLog.h"
#include "log/TRTLogSender.h"
#include "task/TRTThread.h"

#include <vector>
#include <deque>
#include <memory>
#include <mutex>
#include <utility>


class CreatingObjectsThread : public TRTThread
{
public:
    CreatingObjectsThread(threadContent threadConfig);

    void TaskFunc() override;
    size_t getThreadNum();

private:
    threadContent _threadConfig;
    std::unordered_map<std::string,std::unique_ptr<Object>> _objectMap;
};


class WorkerThreads
{
public:
    WorkerThreads(std::vector<threadContent> threadsConfig, std::vector<actionContent> actionsConfig);
    void run();

private:
    std::vector<threadContent> _threadsConfig;
    std::vector<actionContent> _actionsConfig;

    std::deque<std::unique_ptr<CreatingObjectsThread>> _threadPool;
};


#endif //WORKERTHREADS_H
