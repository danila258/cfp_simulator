#include "WorkerThreads.h"

std::mutex out;

void threadFunction(const dataContainers& threadConfig)
{
    std::deque<std::unique_ptr<customMutex>> mutexArr;

    std::vector<customQueue<int>> queueArr;
    queueArr.reserve(threadConfig.queueCount);

    size_t id = 0;

    for (auto& mParams : threadConfig.mutexes)
    {
        mutexArr.emplace_back(std::unique_ptr<customMutex>( new customMutex(mParams) ));
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

    for (auto& qParams : threadConfig.queue)
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

    for (auto& item : mutexArr)
    {
        std::lock_guard<std::mutex> lock(out);
        std::cout << std::this_thread::get_id() << ' ' << item->_params.name << '\n';
    }

    for (auto& item : queueArr)
    {
        std::lock_guard<std::mutex> lock(out);
        std::cout << std::this_thread::get_id() << ' ' << item._params.name << '\n';
    }
}

void workerThreads(const std::vector<threadConfig>& config)
{
    std::vector<std::thread> threadPool;

    for (auto& threadConfig : config)
    {
        threadPool.emplace_back(threadFunction, std::ref(threadConfig.config));
    }

    for (size_t i = 0; i < threadPool.size(); ++i)
    {
        threadPool[i].join();
    }
}
