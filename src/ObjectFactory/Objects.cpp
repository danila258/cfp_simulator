#include "Objects.h"

const UniversalString TRTMutexObject::_typeName = "TRTMutex";
const UniversalString TRTBinSemaphoreObject::_typeName = "TRTBinSemaphore";
const UniversalString TRTQueObject::_typeName = "TRTQue";
const UniversalString TRTSysTimerObject::_typeName = "TRTSysTimer";
const UniversalString TRTEventObject::_typeName = "TRTEvent";
const UniversalString TRTCondVarObject::_typeName = "TRTCondVar";
const UniversalString TRTSharedMemoryObject::_typeName = "TRTSharedMemory";


void checkArgsCount(const std::vector<UniversalString>& args, size_t size)
{
    if (args.size() != size)
    {
        throw std::runtime_error("incorrect count of arguments");
    }
}


TRTMutexObject::TRTMutexObject(const std::vector<UniversalString>& args) try
{
    checkArgsCount(args, 0);
    _object = {new TRTMutex, DELETER(TRTMutex*)};
}
catch (...)
{
    throw;
}

void TRTMutexObject::call(const UniversalString& funcName, const std::vector<UniversalString>& args)
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
        throw std::runtime_error("unknown function name");
    }
}

const UniversalString& TRTMutexObject::staticTypeName()
{
    return _typeName;
}


TRTBinSemaphoreObject::TRTBinSemaphoreObject(const std::vector<UniversalString>& args) try
{
    checkArgsCount(args, 1);
    _object = {new TRTBinSemaphore( std::stoi(args[0]) ), DELETER(TRTBinSemaphore*)};
}
catch (...)
{
   throw;
}

void TRTBinSemaphoreObject::call(const UniversalString& funcName, const std::vector<UniversalString>& args)
{}

const UniversalString& TRTBinSemaphoreObject::staticTypeName()
{
    return _typeName;
}


TRTQueObject::TRTQueObject(const std::vector<UniversalString>& args) try
{
    checkArgsCount(args, 2);
    _object = {new TRTQue(std::stoi(args[0]), std::stoi(args[1])), DELETER(TRTQue*)};
}
catch (...)
{
    throw;
}

void TRTQueObject::call(const UniversalString& funcName, const std::vector<UniversalString>& args)
{
    if (funcName == "Write")
    {
        int len = args[0]->toInt();
        auto* ptr = new char[len];

        CALL(TRTQue*, Write)(ptr, len);
    }
    else if (funcName == "Read")
    {
        int len = args[0]->toInt();
        auto* ptr = new char[len];

        CALL(TRTQue*, Read)(ptr, len);
    }
    else
    {
        throw std::runtime_error("unknown function name");
    }
}

const UniversalString& TRTQueObject::staticTypeName()
{
    return _typeName;
}


TRTSysTimerObject::TRTSysTimerObject(const std::vector<UniversalString>& args) try
{
    checkArgsCount(args, 0);
    _object = {new TRTSysTimer(), DELETER(TRTSysTimer*)};
}
catch (...)
{
    throw;
}

void TRTSysTimerObject::call(const UniversalString& funcName, const std::vector<UniversalString>& args)
{
    if (funcName == "Start")
    {
        int delay = args[0]->toInt();
        CALL(TRTSysTimer*, Start)(delay);
    }
    else if (funcName == "Stop")
    {
        CALL(TRTSysTimer*, Stop)();
    }
    else
    {
        throw std::runtime_error("unknown function name");
    }
}

const UniversalString& TRTSysTimerObject::staticTypeName()
{
    return _typeName;
}


TRTEventObject::TRTEventObject(const std::vector<UniversalString>& args) try
{
    checkArgsCount(args, 2);
    _object = {new TRTEvent(std::stoi(args[0]), std::stoi(args[1])), DELETER(TRTEvent*)};
}
catch (...)
{
    throw;
}

void TRTEventObject::call(const UniversalString& funcName, const std::vector<UniversalString>& args)
{}

const UniversalString& TRTEventObject::staticTypeName()
{
    return _typeName;
}


TRTCondVarObject::TRTCondVarObject(const std::vector<UniversalString>& args) try
{
    checkArgsCount(args, 0);
    _object = {new TRTCondVar(), DELETER(TRTCondVar*)};
}
catch (...)
{
    throw;
}

void TRTCondVarObject::call(const UniversalString& funcName, const std::vector<UniversalString>& args)
{}

const UniversalString& TRTCondVarObject::staticTypeName()
{
    return _typeName;
}


TRTSharedMemoryObject::TRTSharedMemoryObject(const std::vector<UniversalString>& args) try
{
    checkArgsCount(args, 3);
    _object = {new TRTSharedMemory(args[0]->toStdString().data(), std::stoi(args[1]), std::stoi(args[2])), DELETER(TRTSharedMemory*)};
}
catch (...)
{
    throw;
}

void TRTSharedMemoryObject::call(const UniversalString& funcName, const std::vector<UniversalString>& args)
{}

const UniversalString& TRTSharedMemoryObject::staticTypeName()
{
    return _typeName;
}