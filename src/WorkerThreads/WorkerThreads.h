#ifndef WORKERTHREADS_H
#define WORKERTHREADS_H

#include "ConfigParser.h"

#include "log/TRTLog.h"
#include "log/TRTLogSender.h"
#include "task/TRTThread.h"
#include "sync/TRTMutex.h"
#include "sync/TRTBinSemaphore.h"
#include "data/TRTQue.h"
#include "timer/TRTSysTimer.h"

#include <deque>
#include <thread>
#include <memory>


class CreatingObjectsThread : public TRTThread
{
public:
    CreatingObjectsThread(threadConfig threadConfig);

    void TaskFunc() override;
    int getThreadNum();

private:
    threadConfig _threadConfig;
};


void workerThreads(const std::vector<threadConfig>& config);


#endif // WORKERTHREADS_H
