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
    CreatingObjectsThread(threadContent& threadConfig, std::vector<actionContent>& actions);

    void TaskFunc() override;
    size_t getThreadNum();

private:
    const threadContent& _thread;
    std::vector<actionContent>& _actions;

    std::unordered_map<size_t,std::unique_ptr<Object>> _objectMap;
};


class WorkerThreads
{
public:
    WorkerThreads(const std::vector<threadContent>& threadsConfig, const std::vector<actionContent>& actionsConfig);
    void run();

private:
    std::vector<threadContent> _threadsConfig;
    std::vector<actionContent> _actionsConfig;

    std::deque<std::unique_ptr<CreatingObjectsThread>> _threadPool;
};


#endif //WORKERTHREADS_H
