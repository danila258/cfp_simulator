#include "CustomMutex.h"

CustomMutex::CustomMutex(const mutexParams& params) : std::mutex(), _params(params)
{}

void CustomMutex::lock()
{
    std::mutex::lock();
    _params.lock = true;
}

bool CustomMutex::try_lock()
{
    _params.lock = mutex::try_lock();
    return _params.lock;
}

void CustomMutex::unlock()
{
    std::mutex::unlock();
    _params.lock = false;
}
