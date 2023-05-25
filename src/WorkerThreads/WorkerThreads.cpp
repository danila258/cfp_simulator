#include "WorkerThreads.h"

std::mutex loggerMutex;
ObjectFactory factory;

CreatingObjectsThread::CreatingObjectsThread(threadContent& threadConfig, std::vector<actionContent>& actions)
    : _thread(threadConfig), _actions(actions)
{}

void CreatingObjectsThread::TaskFunc()
{
    // separator for log
    std::string sep = "     ";

    loggerMutex.lock();
    rtlog(INFO) << "Create: thread_" << getThreadNum();
    loggerMutex.unlock();

    for (auto& object : _thread.objects)
    {
        size_t id = object.startId;

        for (size_t count = 0; count < object.count; ++count)
        {
            _objectMap.emplace(id, factory.create(object.className, object.args));
            _objectMap[id]->call("SetName", {object.varName->toStdString()});

            loggerMutex.lock();
            rtlog(INFO) << "Thread: " << _thread.number << sep << "Create: " << object.varName->toStdString() << sep << "ID: " << id;
            loggerMutex.unlock();

            ++id;
        }
    }

    for (auto& action : _actions)
    {
        _objectMap[action.id]->call(action.funcName, action.args);

        loggerMutex.lock();
        rtlog(INFO) << "Thread: " << _thread.number << sep << "Class: " << action.className->toStdString() << sep
                    << "ID: " << action.id << sep << "Action: " << action.funcName->toStdString();
        loggerMutex.unlock();

        MillySleep(action.pause);
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
    std::vector<std::vector<actionContent>> actionsArr{_threadsConfig.size()};

    for (const auto& action : _actionsConfig)
    {
        actionsArr[action.thread].emplace_back(action);
    }

    for (auto& thread : _threadsConfig)
    {
        _threadPool.emplace_back( std::unique_ptr<CreatingObjectsThread>(new CreatingObjectsThread(thread, actionsArr[thread.number])) );

        if ( !_threadPool.back()->Start() )
        {
            loggerMutex.lock();
            rtlog(CRITICAL) << "Not created: "<< "thread_" << thread.number;
            loggerMutex.unlock();
        }
    }

    for (auto& thread : _threadPool)
    {
        thread->Join();

        loggerMutex.lock();
        rtlog(INFO) << "Join: "<< "thread_" << thread->getThreadNum();
        loggerMutex.unlock();
    }
}
