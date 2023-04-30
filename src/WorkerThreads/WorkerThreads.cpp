#include "WorkerThreads.h"

std::mutex loggerMutex;
ObjectFactory factory;

CreatingObjectsThread::CreatingObjectsThread(threadContent threadConfig) : _threadConfig(std::move(threadConfig))
{}

void CreatingObjectsThread::TaskFunc()
{
    loggerMutex.lock();
    rtlog(INFO) << "Create thread_" << getThreadNum();
    loggerMutex.unlock();

    for (auto& objectConfig : _threadConfig.objects)
    {
        _objectMap.emplace(objectConfig.varName, factory.create(objectConfig.className, objectConfig.args));

        loggerMutex.lock();
        rtlog(INFO) << "thread_pid: " << _threadConfig.number << " Create " << objectConfig.varName->toStdString();
        loggerMutex.unlock();
    }
}

size_t CreatingObjectsThread::getThreadNum()
{
    return _threadConfig.number;
}


WorkerThreads::WorkerThreads(std::vector<threadContent> threadsConfig, std::vector<actionContent> actionsConfig)
    : _threadsConfig(std::move(threadsConfig)), _actionsConfig(std::move(actionsConfig))
{
    // register all types
    factory.registerType<TRTMutexObject>();
    factory.registerType<TRTBinSemaphoreObject>();
    factory.registerType<TRTQueObject>();
    factory.registerType<TRTSysTimerObject>();
    factory.registerType<TRTEventObject>();
    factory.registerType<TRTCondVarObject>();
    factory.registerType<TRTSharedMemoryObject>();
}

void WorkerThreads::run()
{
    for (auto& threadConfig : _threadsConfig)
    {
        _threadPool.emplace_back( std::unique_ptr<CreatingObjectsThread>(new CreatingObjectsThread(threadConfig)) );

        if ( !_threadPool.back()->Start() )
        {
            loggerMutex.lock();
            rtlog(CRITICAL) << "thread_" << threadConfig.number << " was not created";
            loggerMutex.unlock();
        }
    }

    for (auto& thread : _threadPool)
    {
        thread->Join();

        loggerMutex.lock();
        rtlog(INFO) << "thread_" << thread->getThreadNum() << " join";
        loggerMutex.unlock();
    }
}
