#include "ObjectFactory.h"

const std::string TRTMutexObject::_typeName = "TRTMutex";


void checkArgsCount(const std::vector<std::string>& args, int size)
{
    if (args.size() != size)
    {
        throw std::runtime_error("incorrect count of arguments");
    }
}


TRTMutexObject::TRTMutexObject(const std::vector<std::string>& args)
{
    checkArgsCount(args, 0);
    _object = {new TRTMutex, DELETER(TRTMutex*)};
}

void TRTMutexObject::call(const std::string& funcName, const std::vector<std::string>& args)
{
    if (funcName == "Lock")
    {
    CALL(TRTMutex*, Lock);
    }
    else if (funcName == "Unlock")
    {
    CALL(TRTMutex*, Unlock);
    }
    else
    {
    throw std::runtime_error("unknown func");
    }
}

const std::string& TRTMutexObject::staticTypeName()
{
    return _typeName;
}

std::unique_ptr<Object> ObjectFactory::create(const std::string& className, const std::vector<std::string>& args)
{
    auto it = _typeMap.find(className);

    if (it == _typeMap.end())
    {
        throw std::runtime_error("unknown type");
    }

    return (it->second)(args);
}
