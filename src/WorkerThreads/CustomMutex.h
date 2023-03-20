#ifndef CUSTOMMUTEX_H
#define CUSTOMMUTEX_H

#include <thread>


struct mutexParams
{
    int id;
    std::string name = "mutex";
    bool lock;
};


class CustomMutex : public std::mutex
{
public:
    CustomMutex(const mutexParams& params);

    void lock();
    bool try_lock();
    void unlock();

    mutexParams _params;
};

#endif //CUSTOMMUTEX_H
