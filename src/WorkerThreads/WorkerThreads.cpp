#include "WorkerThreads.h"

std::mutex loggerMutex;

void threadFunction(const threadConfig& threadConfig)
{
    loggerMutex.lock();
    rtlog(INFO) << "Create thread_" << threadConfig.number;
    loggerMutex.unlock();

    std::deque<std::unique_ptr<CustomMutex>> mutexArr;

    std::vector<CustomQueue<int>> queueArr;
    queueArr.reserve(threadConfig.config.queueCount);

    size_t id = 0;

    for (auto& mParams : threadConfig.config.mutexes)
    {
        mutexArr.emplace_back(std::unique_ptr<CustomMutex>(new CustomMutex(mParams) ));
        mutexArr.back()->_params.lock = mParams.lock;
        mutexArr.back()->_params.id = id;
        mutexArr.back()->_params.name += " " + std::to_string(id);
        ++id;

        if (mutexArr.back()->_params.lock)
        {
            mutexArr.back()->lock();
        }

        std::lock_guard<std::mutex> lock(loggerMutex);
        rtlog(INFO) << "thread_pid: " << threadConfig.number << " Create " << mParams.name;
    }

    id = 0;

    for (auto& qParams : threadConfig.config.queue)
    {
        queueArr.emplace_back();
        queueArr.back()._params = qParams;
        queueArr.back()._params.id = id;
        queueArr.back()._params.name += " " + std::to_string(id);
        ++id;

        for (int i = 0; i < qParams.size; ++i)
        {
            queueArr.back().emplace(0);
        }

        std::lock_guard<std::mutex> lock(loggerMutex);
        rtlog(INFO) << "thread_pid: " << threadConfig.number << " Create " << qParams.name;
    }
}

void workerThreads(const std::vector<threadConfig>& config)
{
    std::vector<std::pair<std::thread, int>> threadPool;

    for (auto& threadConfig : config)
    {
        std::thread t(threadFunction, std::ref(threadConfig));
        threadPool.emplace_back(std::move(t), threadConfig.number);
    }

    for (auto& thread : threadPool)
    {
        thread.first.join();

        std::lock_guard<std::mutex> lock(loggerMutex);
        rtlog(INFO) << "thread_" << thread.second << " join";
    }
}
