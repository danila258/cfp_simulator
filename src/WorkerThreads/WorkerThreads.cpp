#include "WorkerThreads.h"

std::mutex loggerMutex;

void threadFunction(const threadConfig& threadConfig, const std::shared_ptr<spdlog::logger>& logger)
{
    logger->info("Create thread_{}", threadConfig.number);

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
        logger->info("thread_pid: {} Create {}", threadConfig.number, mParams.name);
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
        logger->info("thread_pid: {} Create {}", threadConfig.number, qParams.name);
    }
}

void workerThreads(const std::vector<threadConfig>& config, const std::shared_ptr<spdlog::logger>& logger)
{
    std::vector<std::pair<std::thread, int>> threadPool;

    for (auto& threadConfig : config)
    {
        std::thread t(threadFunction, std::ref(threadConfig), std::ref(logger));
        threadPool.emplace_back(std::move(t), threadConfig.number);
    }

    for (auto& item : threadPool)
    {
        item.first.join();
        logger->info("thread_{} join", item.second);
    }
}
