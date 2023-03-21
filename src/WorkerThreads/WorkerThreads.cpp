#include "WorkerThreads.h"

std::mutex loggerMutex;

void threadFunction(const threadConfig& threadConfig, const std::shared_ptr<spdlog::logger>& logger)
{
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
    }

    logger->info("Create thread_{}", threadConfig.number);

    for (auto& item : mutexArr)
    {
        std::lock_guard<std::mutex> lock(loggerMutex);
        logger->info("thread_pid: {} Create {}", threadConfig.number, item->_params.name);
    }

    for (auto& item : queueArr)
    {
        std::lock_guard<std::mutex> lock(loggerMutex);
        logger->info("thread_pid: {} Create {}", threadConfig.number, item._params.name);
    }
}

void workerThreads(const std::vector<threadConfig>& config, const std::shared_ptr<spdlog::logger>& logger)
{
    std::vector<std::thread> threadPool;

    for (auto& threadConfig : config)
    {
        threadPool.emplace_back(threadFunction, std::ref(threadConfig), std::ref(logger));
        threadPool.back().join();
    }
}
