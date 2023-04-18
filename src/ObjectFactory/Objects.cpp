#include "Objects.h"

const std::string TRTMutexObject::_typeName = "TRTMutex";
const std::string TRTBinSemaphoreObject::_typeName = "TRTBinSemaphore";
const std::string TRTQueObject::_typeName = "TRTQue";
const std::string TRTSysTimerObject::_typeName = "TRTSysTimer";
const std::string TRTEventObject::_typeName = "TRTEvent";
const std::string TRTCondVarObject::_typeName = "TRTCondVar";
const std::string TRTSharedMemoryObject::_typeName = "TRTSharedMemory";


void checkArgsCount(const std::vector<std::string>& args, int size)
{
    if (args.size() != size)
    {
        throw std::runtime_error("incorrect count of arguments");
    }
}


TRTMutexObject::TRTMutexObject(const std::vector<std::string>& args) try
{
    checkArgsCount(args, 0);
    _object = {new TRTMutex, DELETER(TRTMutex*)};
}
catch (...)
{
    throw;
}

void TRTMutexObject::call(const std::string& funcName, const std::vector<std::string>& args)
{
    if (funcName == "Lock")
    {
        CALL(TRTMutex*, Lock)();
    }
    else if (funcName == "Unlock")
    {
        CALL(TRTMutex*, Unlock)();
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


TRTBinSemaphoreObject::TRTBinSemaphoreObject(const std::vector<std::string>& args) try
{
    checkArgsCount(args, 1);
    _object = {new TRTBinSemaphore( std::stoi(args[0]) ), DELETER(TRTBinSemaphore*)};
}
catch (...)
{
   throw;
}

void TRTBinSemaphoreObject::call(const std::string& funcName, const std::vector<std::string>& args)
{}

const std::string& TRTBinSemaphoreObject::staticTypeName()
{
    return _typeName;
}


TRTQueObject::TRTQueObject(const std::vector<std::string>& args) try
{
    checkArgsCount(args, 2);
    _object = {new TRTQue(std::stoi(args[0]), std::stoi(args[1])), DELETER(TRTQue*)};
}
catch (...)
{
    throw;
}

void TRTQueObject::call(const std::string& funcName, const std::vector<std::string>& args)
{}

const std::string& TRTQueObject::staticTypeName()
{
    return _typeName;
}


TRTSysTimerObject::TRTSysTimerObject(const std::vector<std::string>& args) try
{
    checkArgsCount(args, 0);
    _object = {new TRTSysTimer(), DELETER(TRTSysTimer*)};
}
catch (...)
{
    throw;
}

void TRTSysTimerObject::call(const std::string& funcName, const std::vector<std::string>& args)
{}

const std::string& TRTSysTimerObject::staticTypeName()
{
    return _typeName;
}


TRTEventObject::TRTEventObject(const std::vector<std::string>& args) try
{
    checkArgsCount(args, 2);
    _object = {new TRTEvent(std::stoi(args[0]), std::stoi(args[1])), DELETER(TRTEvent*)};
}
catch (...)
{
    throw;
}

void TRTEventObject::call(const std::string& funcName, const std::vector<std::string>& args)
{}

const std::string& TRTEventObject::staticTypeName()
{
    return _typeName;
}


TRTCondVarObject::TRTCondVarObject(const std::vector<std::string>& args) try
{
    checkArgsCount(args, 0);
    _object = {new TRTCondVar(), DELETER(TRTCondVar*)};
}
catch (...)
{
    throw;
}

void TRTCondVarObject::call(const std::string& funcName, const std::vector<std::string>& args)
{}

const std::string& TRTCondVarObject::staticTypeName()
{
    return _typeName;
}


TRTSharedMemoryObject::TRTSharedMemoryObject(const std::vector<std::string>& args) try
{
    checkArgsCount(args, 3);
    _object = {new TRTSharedMemory(args[0].c_str(), std::stoi(args[1]), std::stoi(args[2])), DELETER(TRTSharedMemory*)};
}
catch (...)
{
    throw;
}

void TRTSharedMemoryObject::call(const std::string& funcName, const std::vector<std::string>& args)
{}

const std::string& TRTSharedMemoryObject::staticTypeName()
{
    return _typeName;
}