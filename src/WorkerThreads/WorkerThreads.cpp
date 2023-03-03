#include "WorkerThreads.h"

std::mutex out;

void threadFunction(const std::vector<dataStructure>& threadConfig)
{
    out.lock();
    std::cout << std::this_thread::get_id() << '\n';
    out.unlock();

    for (auto& item : threadConfig)
    {
        std::lock_guard<std::mutex> lock(out);
        std::cout << item.name << ' ' << item.count << '\n';
    }

    std::lock_guard<std::mutex> lock(out);
    std::cout << '\n';
}

void workerThreads(const std::vector<threadConfig>& config)
{
    std::vector<std::thread> threadPool;

    for (auto& threadConfig : config)
    {
        threadPool.emplace_back(threadFunction, std::ref(threadConfig.threadContent));
    }

    for (size_t i = 0; i < threadPool.size(); ++i)
    {
        threadPool[i].join();
    }
}
