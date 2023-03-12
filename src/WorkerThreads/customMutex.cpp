#include "customMutex.h"

customMutex::customMutex(const mutexParams& params) : std::mutex(), _params(params)
{}

void customMutex::lock()
{
    std::mutex::lock();
    _params.lock = true;
}

bool customMutex::try_lock()
{
    _params.lock = mutex::try_lock();
    return _params.lock;
}

void customMutex::unlock()
{
    std::mutex::unlock();
    _params.lock = false;
}
