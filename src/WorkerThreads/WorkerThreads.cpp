#include "WorkerThreads.h"

TRTMutex loggerMutex;

CreatingObjectsThread::CreatingObjectsThread(threadConfig threadConfig) : _threadConfig(std::move(threadConfig))
{}

int CreatingObjectsThread::getThreadNum()
{
    return _threadConfig.number;
}

void CreatingObjectsThread::TaskFunc()
{
    loggerMutex.Lock();
    rtlog(INFO) << "Create thread_" << _threadConfig.number;
    loggerMutex.Unlock();

    std::deque<TRTMutex> mutexArr;
    size_t id = 0;

    for (auto& mParams : _threadConfig.config.mutexes)
    {
        mutexArr.emplace_back();
        mutexArr.back().setName(mParams.name + " " + std::to_string(id));
        ++id;

        if (mParams.lock)
        {
            mutexArr.back().Lock();
        }

        loggerMutex.Lock();
        rtlog(INFO) << "thread_pid: " << _threadConfig.number << " Create " << mutexArr.back().getName();
        loggerMutex.Unlock();
    }

    std::deque<TRTBinSemaphore> semaphoreArr;
    id = 0;

    for (auto& sParams : _threadConfig.config.semaphores)
    {
        semaphoreArr.emplace_back(sParams.state);
        semaphoreArr.back().setName(sParams.name + " " + std::to_string(id));
        ++id;

        loggerMutex.Lock();
        rtlog(INFO) << "thread_pid: " << _threadConfig.number << " Create " << semaphoreArr.back().getName();
        loggerMutex.Unlock();
    }

    std::deque<TRTQue> queueArr;
    id = 0;

    for (auto& qParams : _threadConfig.config.queues)
    {
        queueArr.emplace_back(qParams.size, 8);
        queueArr.back().setName(qParams.name + " " + std::to_string(id));
        ++id;

        for (int i = 0; i < qParams.size; ++i)
        {
            int* buf = new int();
            queueArr.back().Write(buf, sizeof(int), this);
        }

        loggerMutex.Lock();
        rtlog(INFO) << "thread_pid: " << _threadConfig.number << " Create " << queueArr.back().getName();
        loggerMutex.Unlock();
    }

    std::deque<TRTSysTimer> timerArr;
    id = 0;

    for (auto& tParams : _threadConfig.config.timers)
    {
        timerArr.emplace_back();
        timerArr.back().setName(tParams.name + " " + std::to_string(id));
        ++id;

        if (tParams.state)
        {
            timerArr.back().Start(tParams.timeout);
        }

        loggerMutex.Lock();
        rtlog(INFO) << "thread_pid: " << _threadConfig.number << " Create " << timerArr.back().getName();
        loggerMutex.Unlock();
    }
}

void workerThreads(const std::vector<threadConfig>& config)
{
    std::deque<CreatingObjectsThread> threadPool;

    for (auto& threadConfig : config)
    {
        threadPool.emplace_back(threadConfig);

        if ( !threadPool.back().Start() )
        {
            loggerMutex.Lock();
            rtlog(WARNING) << "thread_" << threadConfig.number << " was not created";
            loggerMutex.Unlock();
        }
    }

    for (auto& thread : threadPool)
    {
        thread.Join();

        loggerMutex.Lock();
        rtlog(INFO) << "thread_" << thread.getThreadNum() << " join";
        loggerMutex.Unlock();
    }
}
