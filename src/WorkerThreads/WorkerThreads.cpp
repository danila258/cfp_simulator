#include "WorkerThreads.h"

std::mutex loggerMutex;
ObjectFactory factory;

CreatingObjectsThread::CreatingObjectsThread(threadContent& threadConfig, std::vector<actionContent>& actions)
    : _thread(threadConfig), _actions(actions)
{}

void CreatingObjectsThread::TaskFunc()
{
    loggerMutex.lock();
    rtlog(INFO) << "Create thread_" << getThreadNum();
    loggerMutex.unlock();

    for (auto& object : _thread.objects)
    {
        size_t id = object.startId;

        for (size_t count = 0; count < object.count; ++count)
        {
            _objectMap.emplace(id, factory.create(object.className, object.args));
            _objectMap[id]->call("SetName", {object.varName->toStdString()});

            loggerMutex.lock();
            rtlog(INFO) << "Thread-" << _thread.number << " Create-" << object.varName->toStdString() << " id-" << id;
            loggerMutex.unlock();

            ++id;
        }
    }

    for (auto& action : _actions)
    {
        _objectMap[action.id]->call(action.funcName, action.args);

        loggerMutex.lock();
        rtlog(INFO) << "Thread-" << _thread.number << " Class-" << action.className->toStdString()
                    << " id-" << action.id << " Action-" << action.funcName->toStdString();
        loggerMutex.unlock();
    }
}

size_t CreatingObjectsThread::getThreadNum()
{
    return _thread.number;
}


WorkerThreads::WorkerThreads(const std::vector<threadContent>& threadsConfig, const std::vector<actionContent>& actionsConfig)
    : _threadsConfig(threadsConfig), _actionsConfig(actionsConfig)
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
    std::unordered_map<size_t, std::vector<actionContent>> actionsMap;

    for (auto& action : _actionsConfig)
    {
        actionsMap[action.thread].emplace_back(action);
    }

    for (auto& threadConfig : _threadsConfig)
    {
        _threadPool.emplace_back( std::unique_ptr<CreatingObjectsThread>(new CreatingObjectsThread(threadConfig, actionsMap[threadConfig.number])) );

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
