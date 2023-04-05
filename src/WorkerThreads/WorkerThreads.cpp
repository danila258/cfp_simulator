#include "WorkerThreads.h"

std::mutex loggerMutex;

CreatingObjectsThread::CreatingObjectsThread(threadConfig threadConfig) : _threadConfig(std::move(threadConfig))
{}

int CreatingObjectsThread::getThreadNum()
{
    return _threadConfig.number;
}

void CreatingObjectsThread::TaskFunc()
{
    loggerMutex.lock();
    rtlog(INFO) << "Create thread_" << _threadConfig.number;
    loggerMutex.unlock();

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

        loggerMutex.lock();
        rtlog(INFO) << "thread_pid: " << _threadConfig.number << " Create " << mutexArr.back().getName();
        loggerMutex.unlock();
    }

    std::deque<TRTBinSemaphore> semaphoreArr;
    id = 0;

    for (auto& sParams : _threadConfig.config.semaphores)
    {
        semaphoreArr.emplace_back(sParams.state);
        semaphoreArr.back().setName(sParams.name + " " + std::to_string(id));
        ++id;

        loggerMutex.lock();
        rtlog(INFO) << "thread_pid: " << _threadConfig.number << " Create " << semaphoreArr.back().getName();
        loggerMutex.unlock();
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

        loggerMutex.lock();
        rtlog(INFO) << "thread_pid: " << _threadConfig.number << " Create " << queueArr.back().getName();
        loggerMutex.unlock();
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

        loggerMutex.lock();
        rtlog(INFO) << "thread_pid: " << _threadConfig.number << " Create " << timerArr.back().getName();
        loggerMutex.unlock();
    }

    std::deque<TRTEvent> eventArr;
    id = 0;

    for (auto& eParams : _threadConfig.config.events)
    {
        eventArr.emplace_back(eParams.fState);
        eventArr.back().SetName(eParams.name + " " + std::to_string(id));
        ++id;

        loggerMutex.lock();
        rtlog(INFO) << "thread_pid: " << _threadConfig.number << " Create " << timerArr.back().getName();
        loggerMutex.unlock();
    }

    std::deque<TRTCondVar> condvarArr;
    id = 0;

    for (auto& cParams : _threadConfig.config.condvars)
    {
        condvarArr.emplace_back();
        condvarArr.back().setName(cParams.name + " " + std::to_string(id));
        ++id;

        loggerMutex.lock();
        rtlog(INFO) << "thread_pid: " << _threadConfig.number << " Create " << timerArr.back().getName();
        loggerMutex.unlock();
    }

    std::deque<TRTSharedMemory> memoryArr;
    id = 0;

    for (auto& mParams : _threadConfig.config.memory)
    {
        memoryArr.emplace_back(std::string(mParams.name + " " + std::to_string(id)).c_str(), mParams.lSize, mParams.bMap);
        ++id;

        if (mParams.timeout > 0)
        {
            memoryArr.back().Lock(mParams.timeout);
        }

        loggerMutex.lock();
        rtlog(INFO) << "thread_pid: " << _threadConfig.number << " Create " << timerArr.back().getName();
        loggerMutex.unlock();
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
            loggerMutex.lock();
            rtlog(WARNING) << "thread_" << threadConfig.number << " was not created";
            loggerMutex.unlock();
        }
    }

    for (auto& thread : threadPool)
    {
        thread.Join();

        loggerMutex.lock();
        rtlog(INFO) << "thread_" << thread.getThreadNum() << " join";
        loggerMutex.unlock();
    }
}
